// Copyright René Ferdinand Rivera Morell
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_ARGUMENTS_HPP
#define LYRA_ARGUMENTS_HPP

#include "lyra/detail/print.hpp"
#include "lyra/detail/tokens.hpp"
#include "lyra/detail/trait_utils.hpp"
#include "lyra/option_style.hpp"
#include "lyra/parser.hpp"
#include "lyra/parser_result.hpp"
#include "lyra/printer.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace lyra {

/* tag::reference[]

[#lyra_arguments]
= `lyra::arguments`

A Combined parser made up of any number of parsers. Creating and using
one of these as a basis one can incrementally compose other parsers into this
one. For example:

[source]
----
auto p = lyra::arguments();
std::string what;
float when = 0;
std::string where;
p |= lyra::opt(what, "what")["--make-it-so"]("Make it so.").required();
p |= lyra::opt(when. "when")["--time"]("When to do <what>.").optional();
p.add_argument(lyra::opt(where, "where").name("--where")
	.help("There you are.").optional());
----

*/ // end::reference[]
class arguments : public parser
{
	public:
	// How to evaluate the collection of arguments within the limits of the
	// cardinality.
	enum evaluation
	{
		// Any of the arguments, in any order, are valid. I.e. an inclusive-or.
		eval_any = 0,
		// All arguments, in sequence, matched. I.e. conjunctive-and.
		eval_sequence = 1,
		// Any of the arguments, in any order, are valid and uknown arguments
		// do not cause parsing errors.
		eval_relaxed = 2,
	};

	arguments() = default;

	arguments(evaluation e)
		: eval_mode(e)
	{}

	// Copy construction, needs to copy the the composed parsers.
	arguments(const arguments & other);

	// Compose a regular parser.
	arguments & add_argument(parser const & p);
	arguments & operator|=(parser const & p);

	// Compose the parsers from another `arguments`.
	arguments & add_argument(arguments const & other);
	arguments & operator|=(arguments const & other);

	// Concat composition.
	template <typename T, typename U>
	friend typename std::enable_if<
		std::is_base_of<arguments,
			typename detail::remove_cvref<T>::type>::value,
		T &>::type
		operator|(T & self, U const & other)
	{
		return static_cast<T &>(self.add_argument(other));
	}
	template <typename T, typename U>
	friend typename std::enable_if<
		std::is_base_of<arguments,
			typename detail::remove_cvref<T>::type>::value,
		T &>::type
		operator|(T && self, U const & other)
	{
		return static_cast<T &>(self.add_argument(other));
	}

	// Parsing mode.
	arguments & sequential();
	arguments & inclusive();
	arguments & relaxed();

	// Access.
	template <typename T>
	T & get(std::size_t i);

	// Internal..

	std::string get_usage_text(const option_style & style) const override
	{
		std::string text;
		for (auto const & p : parsers)
		{
			std::string usage_text = p->get_usage_text(style);
			if (usage_text.size() > 0)
			{
				if (!text.empty()) text += " ";
				if (p->is_group() && p->is_optional())
					((text += "[ ") += usage_text) += " ]";
				else if (p->is_group())
					((text += "{ ") += usage_text) += " }";
				else if (p->is_optional())
					((text += "[") += usage_text) += "]";
				else
					text += usage_text;
			}
		}
		return text;
	}

	std::string get_description_text(const option_style & style) const override
	{
		std::string text;
		for (auto const & p : parsers)
		{
			if (p->is_group()) continue;
			auto child_description = p->get_description_text(style);
			if (!child_description.empty())
			{
				if (!text.empty()) text += "\n";
				text += child_description;
			}
		}
		return text;
	}

	detail::parser_cardinality cardinality() const override { return { 0, 0 }; }

	result validate() const override
	{
		for (auto const & p : parsers)
		{
			auto parse_valid = p->validate();
			if (!parse_valid) return parse_valid;
		}
		return result::ok();
	}

	parse_result parse(detail::token_iterator const & tokens,
		const option_style & style) const override
	{
		switch (eval_mode)
		{
			case eval_any:
			case eval_relaxed: return parse_any(tokens, style);
			case eval_sequence: return parse_sequence(tokens, style);
		}
		return parse_result::error(
			detail::parse_state(parser_result_type::no_match, tokens),
			"Unknown evaluation mode; not one of 'any', 'sequence', or 'relaxed'.");
	}

	// Match in any order, any number of times. Returns an error if nothing
	// matched.
	parse_result parse_any(
		detail::token_iterator const & tokens, const option_style & style) const
	{
		LYRA_PRINT_SCOPE("arguments::parse_any");

		std::vector<std::size_t> parsing_count(parsers.size(), 0);
		auto parsing_result = parse_result::ok(
			detail::parse_state(parser_result_type::empty_match, tokens));
		auto nomatch_result = parse_result::ok(
			detail::parse_state(parser_result_type::no_match, tokens));

		while (parsing_result.value().remainingTokens())
		{
			LYRA_PRINT_DEBUG("(?)", get_usage_text(style), "?=",
				parsing_result.value().remainingTokens()
					? parsing_result.value().remainingTokens().argument().name
					: "",
				"..");
			bool token_parsed = false;

			auto parsing_count_i = parsing_count.begin();
			for (auto & p : parsers)
			{
				auto parser_cardinality = p->cardinality();
				if (parser_cardinality.is_unbounded()
					|| *parsing_count_i < parser_cardinality.maximum)
				{
					auto subparse_result = p->parse(
						parsing_result.value().remainingTokens(), style);
					if (!subparse_result)
					{
						LYRA_PRINT_DEBUG("(!)", get_usage_text(style), "!=",
							parsing_result.value()
								.remainingTokens()
								.argument()
								.name);
						// Is the subparse error bad enough to trigger an
						// immediate return? For example for an option syntax
						// error.
						if (subparse_result.has_value()
							&& subparse_result.value().type()
								== parser_result_type::short_circuit_all)
							return subparse_result;
						// For not severe errors, we save the error if it's
						// the first so that in case no other parsers match
						// we can report the earliest problem, as that's
						// the likeliest issue.
						else if (nomatch_result)
							nomatch_result = parse_result(subparse_result);
					}
					else if (subparse_result
						&& subparse_result.value().type()
							== parser_result_type::empty_match)
					{
						LYRA_PRINT_DEBUG("(=)", get_usage_text(style), "==",
							parsing_result.value()
								.remainingTokens()
								.argument()
								.name,
							"==>", subparse_result.value().type());
						parsing_result = parse_result(subparse_result);
					}
					else if (subparse_result
						&& subparse_result.value().type()
							!= parser_result_type::no_match)
					{
						LYRA_PRINT_DEBUG("(=)", get_usage_text(style), "==",
							parsing_result.value()
								.remainingTokens()
								.argument()
								.name,
							"==>", subparse_result.value().type());
						parsing_result = parse_result(subparse_result);
						token_parsed = true;
						*parsing_count_i += 1;
						break;
					}
				}
				++parsing_count_i;
			}

			if (parsing_result.value().type()
				== parser_result_type::short_circuit_all)
				return parsing_result;
			if (!token_parsed)
			{
				// Nothing matched for the current arg and we are doing relaxed
				// parsing. Hence we need to skip over that unknown arg to
				// continue trying the rest.
				if (eval_mode == eval_relaxed)
				{
					LYRA_PRINT_DEBUG("(=)", get_usage_text(style), "==",
						parsing_result.value()
							.remainingTokens()
							.argument()
							.name,
						"==> skipped");
					auto remainingTokens
						= parsing_result.value().remainingTokens();
					remainingTokens.pop(remainingTokens.argument());
					parsing_result = parse_result::ok(detail::parse_state(
						parser_result_type::empty_match, remainingTokens));
				}
				// If something signaled and error, and hence we didn't
				// match/parse anything, we indicate the error if not in relaxed
				// mode.
				else if (!nomatch_result)
					return nomatch_result;
				// Encountered something unrecognized. We stop and report the
				// result.
				else
					break;
			}
		}
		// Check missing required options. For bounded arguments we check
		// bound min and max bounds against what we parsed. For the loosest
		// required arguments we check for only the minimum. As the upper
		// bound could be infinite.
		{
			auto parsing_count_i = parsing_count.begin();
			for (auto & p : parsers)
			{
				auto parser_cardinality = p->cardinality();
				if ((parser_cardinality.is_bounded()
						&& (*parsing_count_i < parser_cardinality.minimum
							|| parser_cardinality.maximum < *parsing_count_i))
					|| (parser_cardinality.is_required()
						&& (*parsing_count_i < parser_cardinality.minimum)))
					return make_parse_error(tokens, *p, parsing_result, style);
				++parsing_count_i;
			}
		}
		return parsing_result;
	}

	parse_result parse_sequence(
		detail::token_iterator const & tokens, const option_style & style) const
	{
		LYRA_PRINT_SCOPE("arguments::parse_sequence");
		LYRA_PRINT_DEBUG("(?)", get_usage_text(style),
			"?=", tokens ? tokens.argument().name : "", "..");

		std::vector<std::size_t> parsing_count(parsers.size(), 0);
		auto p_result = parse_result::ok(
			detail::parse_state(parser_result_type::empty_match, tokens));

		// Sequential parsing means we walk through the given parsers in order
		// and exhaust the tokens as we match parsers.
		auto parsing_count_i = parsing_count.begin();
		for (auto & p : parsers)
		{
			auto parser_cardinality = p->cardinality();
			// This is a greedy sequential parsing algo. As it parses the
			// current argument as much as possible.
			do
			{
				auto subresult
					= p->parse(p_result.value().remainingTokens(), style);
				if (!subresult) break;
				if (parser_result_type::no_match == subresult.value().type())
				{
					LYRA_PRINT_DEBUG("(!)", get_usage_text(style), "!=",
						p_result.value().remainingTokens()
							? p_result.value().remainingTokens().argument().name
							: "",
						"==>", subresult.value().type());
					break;
				}
				if (parser_result_type::short_circuit_all
					== subresult.value().type())
					return subresult;
				if (parser_result_type::matched == subresult.value().type())
				{
					LYRA_PRINT_DEBUG("(=)", get_usage_text(style), "==",
						p_result.value().remainingTokens()
							? p_result.value().remainingTokens().argument().name
							: "",
						"==>", subresult.value().type());
					*parsing_count_i += 1;
					p_result = subresult;
				}
				if (parser_result_type::empty_match == subresult.value().type())
				{
					LYRA_PRINT_DEBUG("(=)", get_usage_text(style), "==",
						p_result.value().remainingTokens()
							? p_result.value().remainingTokens().argument().name
							: "",
						"==>", subresult.value().type());
					*parsing_count_i += 1;
				}
			}
			while (p_result.value().have_tokens()
				&& (parser_cardinality.is_unbounded()
					|| *parsing_count_i < parser_cardinality.maximum));
			// Check missing required options immediately as for sequential the
			// argument is greedy and will fully match here. For bounded
			// arguments we check bound min and max bounds against what we
			// parsed. For the loosest required arguments we check for only the
			// minimum. As the upper bound could be infinite.
			if ((parser_cardinality.is_bounded()
					&& (*parsing_count_i < parser_cardinality.minimum
						|| parser_cardinality.maximum < *parsing_count_i))
				|| (parser_cardinality.is_required()
					&& (*parsing_count_i < parser_cardinality.minimum)))
				return make_parse_error(tokens, *p, p_result, style);
			++parsing_count_i;
		}
		// The return is just the last state as it contains any remaining tokens
		// to parse.
		return p_result;
	}

	template <typename R>
	parse_result make_parse_error(const detail::token_iterator & tokens,
		const parser & p,
		const R & p_result,
		const option_style & style) const
	{
		if (tokens)
			return parse_result::error(p_result.value(),
				"Unrecognized argument '" + tokens.argument().name
					+ "' while parsing: " + p.get_usage_text(style));
		else
			return parse_result::error(
				p_result.value(), "Expected: " + p.get_usage_text(style));
	}

	std::unique_ptr<parser> clone() const override
	{
		return make_clone<arguments>(this);
	}

	template <typename T>
	T & print_help(T & os) const
	{
		std::unique_ptr<printer> p = make_printer(os);
		this->print_help_text(*p, get_option_style());
		return os;
	}

	const parser * get_named(const std::string & n) const override
	{
		for (auto & p : parsers)
		{
			const parser * p_result = p->get_named(n);
			if (p_result) return p_result;
		}
		return nullptr;
	}

	protected:
	std::shared_ptr<option_style> opt_style;
	std::vector<std::unique_ptr<parser>> parsers;
	evaluation eval_mode = eval_any;

	option_style get_option_style() const
	{
		return opt_style ? *opt_style : option_style::posix();
	}

	option_style & ref_option_style()
	{
		if (!opt_style)
			opt_style = std::make_shared<option_style>(option_style::posix());
		return *opt_style;
	}

	void print_help_text_details(
		printer & p, const option_style & style) const override
	{
		for_each_print_ordered_parser(style, parsers.begin(), parsers.end(),
			[&](const option_style & s, const parser & q) {
				q.print_help_text_details(p, s);
			});
	}
};

/* tag::reference[]

[#lyra_arguments_ctor]
== Construction

end::reference[] */

/* tag::reference[]

[#lyra_arguments_ctor_default]
=== Default

[source]
----
arguments() = default;
----

Default constructing a `arguments` is the starting point to adding arguments
and options for parsing a arguments line.

end::reference[] */

/* tag::reference[]

[#lyra_arguments_ctor_copy]
=== Copy

[source]
----
arguments::arguments(const arguments& other);
----

end::reference[] */
inline arguments::arguments(const arguments & other)
	: parser(other)
	, opt_style(other.opt_style)
	, eval_mode(other.eval_mode)
{
	for (auto & other_parser : other.parsers)
	{
		parsers.push_back(other_parser->clone());
	}
}

/* tag::reference[]

[#lyra_arguments_specification]
== Specification

end::reference[] */

// ==

/* tag::reference[]
[#lyra_arguments_add_argument]
=== `lyra::arguments::add_argument`

[source]
----
arguments& arguments::add_argument(parser const& p);
arguments& arguments::operator|=(parser const& p);
arguments& arguments::add_argument(arguments const& other);
arguments& arguments::operator|=(arguments const& other);
----

Adds the given argument parser to the considered arguments for this
`arguments`. Depending on the parser given it will be: directly added as an
argument (for `parser`), or add the parsers from another `arguments` to
this one.

end::reference[] */
inline arguments & arguments::add_argument(parser const & p)
{
	parsers.push_back(p.clone());
	return *this;
}
inline arguments & arguments::operator|=(parser const & p)
{
	return this->add_argument(p);
}
inline arguments & arguments::add_argument(arguments const & other)
{
	if (other.is_group())
	{
		parsers.push_back(other.clone());
	}
	else
	{
		for (auto & p : other.parsers)
		{
			parsers.push_back(p->clone());
		}
	}
	return *this;
}
inline arguments & arguments::operator|=(arguments const & other)
{
	return this->add_argument(other);
}

/* tag::reference[]
=== `lyra::arguments::sequential`

[source]
----
arguments & arguments::sequential();
----

Sets the parsing mode for the arguments to "sequential". When parsing the
arguments they will be, greedily, consumed in the order they where added.
This is useful for sub-commands and structured command lines.

end::reference[] */
inline arguments & arguments::sequential()
{
	eval_mode = eval_sequence;
	return *this;
}

/* tag::reference[]
=== `lyra::arguments::inclusive`

[source]
----
arguments & arguments::inclusive();
----

Sets the parsing mode for the arguments to "inclusively any". This is the
default that attempts to match each parsed argument with all the available
parsers. This means that there is no ordering enforced.

end::reference[] */
inline arguments & arguments::inclusive()
{
	eval_mode = eval_any;
	return *this;
}

/* tag::reference[]
=== `lyra::arguments::relaxed`

[source]
----
arguments & arguments::relaxed();
----

Sets the parsing mode for the arguments to "relaxed any". When parsing the
arguments it attempts to match each parsed argument with all the available
parsers. This means that there is no ordering enforced. Unknown, i.e. failed,
parsing are ignored.

end::reference[] */
inline arguments & arguments::relaxed()
{
	eval_mode = eval_relaxed;
	return *this;
}

/* tag::reference[]
=== `lyra::arguments::get`

[source]
----
template <typename T>
T & arguments::get(std::size_t i);
----

Get a modifiable reference to one of the parsers specified.

end::reference[] */
template <typename T>
T & arguments::get(std::size_t i)
{
	return static_cast<T &>(*parsers.at(i));
}

/* tag::reference[]
=== `lyra::operator<<`

[source]
----
template <typename T>
T & operator<<(T & os, arguments const & a);
----

Prints the help text for the arguments to the given stream `os`. The `os` stream
is not used directly for printing out. Instead a <<lyra_printer>> object is
created by calling `lyra::make_printer(os)`. This indirection allows one to
customize how the output is generated.

end::reference[] */
template <typename T>
T & operator<<(T & os, arguments const & a)
{
	return a.print_help(os);
}

} // namespace lyra

#endif

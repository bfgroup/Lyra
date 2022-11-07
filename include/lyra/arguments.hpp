// Copyright 2018-2022 René Ferdinand Rivera Morell
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_ARGUMENTS_HPP
#define LYRA_ARGUMENTS_HPP

#include "lyra/detail/print.hpp"
#include "lyra/exe_name.hpp"
#include "lyra/parser.hpp"

#include <functional>
#include <sstream>

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
		any = 0,
		// All arguments, in sequence, matched. I.e. conjunctive-and.
		sequence = 1
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
	template <typename T>
	arguments operator|(T const & other) const
	{
		return arguments(*this) |= other;
	}

	// Parsing mode.
	arguments & sequential();
	arguments & inclusive();

	// Access.
	template <typename T>
	T & get(size_t i);

	// Internal..

	virtual std::string get_usage_text(
		const option_style & style) const override
	{
		std::ostringstream os;
		for (auto const & p : parsers)
		{
			std::string usage_text = p->get_usage_text(style);
			if (usage_text.size() > 0)
			{
				if (os.tellp() != std::ostringstream::pos_type(0)) os << " ";
				if (p->is_group() && p->is_optional())
					os << "[ " << usage_text << " ]";
				else if (p->is_group())
					os << "{ " << usage_text << " }";
				else if (p->is_optional())
					os << "[" << usage_text << "]";
				else
					os << usage_text;
			}
		}
		return os.str();
	}

	virtual std::string get_description_text(
		const option_style & style) const override
	{
		std::ostringstream os;
		for (auto const & p : parsers)
		{
			if (p->is_group()) continue;
			auto child_description = p->get_description_text(style);
			if (!child_description.empty()) os << child_description << "\n";
		}
		return os.str();
	}

	// Return a container of the individual help text for the composed parsers.
	virtual help_text get_help_text(const option_style & style) const override
	{
		help_text text;
		for (auto const & p : parsers)
		{
			if (p->is_group()) text.push_back({ "", "" });
			auto child_help = p->get_help_text(style);
			text.insert(text.end(), child_help.begin(), child_help.end());
		}
		return text;
	}

	virtual detail::parser_cardinality cardinality() const override
	{
		return { 0, 0 };
	}

	virtual result validate() const override
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
			case any: return parse_any(tokens, style);
			case sequence: return parse_sequence(tokens, style);
		}
		return parse_result::error(
			detail::parse_state(parser_result_type::no_match, tokens),
			"Unknown evaluation mode; not one of 'any', or 'sequence'.");
	}

	// Match in any order, any number of times. Returns an error if nothing
	// matched.
	parse_result parse_any(
		detail::token_iterator const & tokens, const option_style & style) const
	{
		LYRA_PRINT_SCOPE("arguments::parse_any");
		LYRA_PRINT_DEBUG("(?)", get_usage_text(style),
			"?=", tokens ? tokens.argument().name : "", "..");

		struct ParserInfo
		{
			parser const * parser_p = nullptr;
			size_t count = 0;
		};
		std::vector<ParserInfo> parser_info(parsers.size());
		{
			size_t i = 0;
			for (auto const & p : parsers) parser_info[i++].parser_p = p.get();
		}

		auto p_result = parse_result::ok(
			detail::parse_state(parser_result_type::matched, tokens));
		auto error_result = parse_result::ok(
			detail::parse_state(parser_result_type::no_match, tokens));
		while (p_result.value().remainingTokens())
		{
			bool token_parsed = false;

			for (auto & parse_info : parser_info)
			{
				auto parser_cardinality = parse_info.parser_p->cardinality();
				if (parser_cardinality.is_unbounded()
					|| parse_info.count < parser_cardinality.maximum)
				{
					auto subparse_result = parse_info.parser_p->parse(
						p_result.value().remainingTokens(), style);
					if (!subparse_result)
					{
						LYRA_PRINT_DEBUG("(!)", get_usage_text(style), "!=",
							p_result.value().remainingTokens().argument().name);
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
						if (error_result)
							error_result = parse_result(subparse_result);
					}
					else if (subparse_result
						&& subparse_result.value().type()
							!= parser_result_type::no_match)
					{
						LYRA_PRINT_DEBUG("(=)", get_usage_text(style), "==",
							p_result.value().remainingTokens().argument().name,
							"==>", subparse_result.value().type());
						p_result = parse_result(subparse_result);
						token_parsed = true;
						parse_info.count += 1;
						break;
					}
				}
			}

			if (p_result.value().type() == parser_result_type::short_circuit_all)
				return p_result;
			// If something signaled and error, and hence we didn't match/parse
			// anything, we indicate the error.
			if (!token_parsed && !error_result) return error_result;
			if (!token_parsed) break;
		}
		// Check missing required options. For bounded arguments we check
		// bound min and max bounds against what we parsed. For the loosest
		// required arguments we check for only the minimum. As the upper
		// bound could be infinite.
		for (auto & parseInfo : parser_info)
		{
			auto parser_cardinality = parseInfo.parser_p->cardinality();
			if ((parser_cardinality.is_bounded()
					&& (parseInfo.count < parser_cardinality.minimum
						|| parser_cardinality.maximum < parseInfo.count))
				|| (parser_cardinality.is_required()
					&& (parseInfo.count < parser_cardinality.minimum)))
			{
				return parse_result::error(p_result.value(),
					"Expected: " + parseInfo.parser_p->get_usage_text(style));
			}
		}
		return p_result;
	}

	parse_result parse_sequence(
		detail::token_iterator const & tokens, const option_style & style) const
	{
		LYRA_PRINT_SCOPE("arguments::parse_sequence");
		LYRA_PRINT_DEBUG("(?)", get_usage_text(style),
			"?=", tokens ? tokens.argument().name : "", "..");

		struct ParserInfo
		{
			parser const * parser_p = nullptr;
			size_t count = 0;
		};
		std::vector<ParserInfo> parser_info(parsers.size());
		{
			size_t i = 0;
			for (auto const & p : parsers) parser_info[i++].parser_p = p.get();
		}

		auto p_result = parse_result::ok(
			detail::parse_state(parser_result_type::matched, tokens));

		// Sequential parsing means we walk through the given parsers in order
		// and exhaust the tokens as we match persers.
		for (std::size_t parser_i = 0; parser_i < parsers.size(); ++parser_i)
		{
			auto & parse_info = parser_info[parser_i];
			auto parser_cardinality = parse_info.parser_p->cardinality();
			// This is a greedy sequential parsing algo. As it parsers the
			// current argument as much as possible.
			do
			{
				auto subresult = parse_info.parser_p->parse(
					p_result.value().remainingTokens(), style);
				if (!subresult)
				{
					break;
				}
				if (subresult.value().type()
					== parser_result_type::short_circuit_all)
				{
					return subresult;
				}
				if (subresult.value().type() != parser_result_type::no_match)
				{
					LYRA_PRINT_DEBUG("(=)", get_usage_text(style), "==",
						p_result.value().remainingTokens()
							? p_result.value().remainingTokens().argument().name
							: "",
						"==>", subresult.value().type());
					p_result = subresult;
					parse_info.count += 1;
				}
			}
			while (p_result.value().have_tokens()
				&& (parser_cardinality.is_unbounded()
					|| parse_info.count < parser_cardinality.maximum));
			// Check missing required options immediately as for sequential the
			// argument is greedy and will fully match here. For bounded
			// arguments we check bound min and max bounds against what we
			// parsed. For the loosest required arguments we check for only the
			// minimum. As the upper bound could be infinite.
			if ((parser_cardinality.is_bounded()
					&& (parse_info.count < parser_cardinality.minimum
						|| parser_cardinality.maximum < parse_info.count))
				|| (parser_cardinality.is_required()
					&& (parse_info.count < parser_cardinality.minimum)))
			{
				return parse_result::error(p_result.value(),
					"Expected: " + parse_info.parser_p->get_usage_text(style));
			}
		}
		// The return is just the last state as it contains any remaining tokens
		// to parse.
		return p_result;
	}

	virtual std::unique_ptr<parser> clone() const override
	{
		return make_clone<arguments>(this);
	}

	friend std::ostream & operator<<(
		std::ostream & os, arguments const & parser)
	{
		const option_style & s
			= parser.opt_style ? *parser.opt_style : option_style::posix();
		parser.print_help_text(os, s);
		return os;
	}

	virtual const parser * get_named(const std::string & n) const override
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

	private:
	std::vector<std::unique_ptr<parser>> parsers;
	evaluation eval_mode = any;
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
	eval_mode = sequence;
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
	eval_mode = any;
	return *this;
}

/* tag::reference[]
=== `lyra::arguments::get`

[source]
----
template <typename T>
T & arguments::get(size_t i);
----

Get a modifyable reference to one of the parsers specified.

end::reference[] */
template <typename T>
T & arguments::get(size_t i)
{
	return static_cast<T &>(*parsers.at(i));
}

} // namespace lyra

#endif

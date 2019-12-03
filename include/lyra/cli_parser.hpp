// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_CLI_PARSER_HPP
#define LYRA_CLI_PARSER_HPP

#include "lyra/exe_name.hpp"
#include "lyra/parser.hpp"

namespace lyra
{
/* tag::reference[]

[#lyra_cli_parser]
= `lyra::cli_parser`

A Combined parser made up of any two or more other parsers. Creating and using
one of these as a basis one can incrementally compose other parsers into this
one. For example:

[source]
----
auto cli = lyra::cli_parser();
std::string what;
float when = 0;
std::string where;
cli |= lyra::opt(what, "what")["--make-it-so"]("Make it so.").required();
cli |= lyra::opt(when. "when")["--time"]("When to do <what>.").optional();
cli.add_argument(lyra::opt(where, "where").name("--where")
	.help("There you are.").optional());
----

*/ // end::reference[]
class cli_parser : parser_base
{
	public:
	cli_parser() = default;

	// Copy construction, needs to copy the exe name and the composed parsers.
	cli_parser(const cli_parser& other);

	// Compose the `exe_name` parser.
	cli_parser& add_argument(exe_name const& exe_name);
	cli_parser& operator|=(exe_name const& exe_name);

	// Compose a regular parser.
	cli_parser& add_argument(parser_base const& p);
	cli_parser& operator|=(parser_base const& p);

	// Compose the parsers from another `cli_parser`.
	cli_parser& add_argument(cli_parser const& other);
	cli_parser& operator|=(cli_parser const& other);

	// Concat composition.
	template <typename T>
	cli_parser operator|(T const& other) const
	{
		return cli_parser(*this) |= other;
	}

	// Stream out generates the help output.
	friend auto operator<<(std::ostream& os, cli_parser const& parser)
		-> std::ostream&
	{
		parser.writeToStream(os);
		return os;
	}

	// Parse from arguments.
	parse_result parse(
		args const& args,
		parser_customization const& customize
		= default_parser_customization()) const;

	// Internal..

	// Return a container of the individual help text for the composed parsers.
	virtual help_text get_help_text() const override
	{
		help_text text;
		for (auto const& p : m_parsers)
		{
			auto child_help = p->get_help_text();
			text.insert(text.end(), child_help.begin(), child_help.end());
		}
		return text;
	}

	auto validate() const -> result override
	{
		for (auto const& p : m_parsers)
		{
			auto result = p->validate();
			if (!result) return result;
		}
		return result::ok();
	}

	parse_result parse(
		std::string const& exe_name, detail::token_iterator const& tokens,
		parser_customization const& customize) const override
	{
		struct ParserInfo
		{
			parser_base const* parser = nullptr;
			size_t count = 0;
		};
		std::vector<ParserInfo> parseInfos(m_parsers.size());
		{
			size_t i = 0;
			for (auto const& p : m_parsers) parseInfos[i++].parser = p.get();
		}

		m_exeName.set(exe_name);

		auto result = parse_result::ok(
			detail::parse_state(parser_result_type::no_match, tokens));
		while (result.value().remainingTokens())
		{
			bool tokenParsed = false;

			for (auto& parseInfo : parseInfos)
			{
				auto parser_cardinality = parseInfo.parser->cardinality();
				if (parser_cardinality.is_unbounded()
					|| parseInfo.count < parser_cardinality.maximum)
				{
					result = parseInfo.parser->parse(
						exe_name, result.value().remainingTokens(), customize);
					if (!result) return result;
					if (result.value().type() != parser_result_type::no_match)
					{
						tokenParsed = true;
						++parseInfo.count;
						break;
					}
				}
			}

			if (result.value().type() == parser_result_type::short_circuit_all)
				return result;
			if (!tokenParsed)
				return parse_result::runtimeError(
					result.value(),
					"Unrecognized token: "
						+ result.value().remainingTokens().argument().name);
		}
		// Check missing required options. For bounded arguments we check
		// bound min and max bounds against what we parsed. For the loosest
		// required arguments we check for only the minimum. As the upper
		// bound could be infinite.
		for (auto& parseInfo : parseInfos)
		{
			auto parser_cardinality = parseInfo.parser->cardinality();
			if ((parser_cardinality.is_bounded()
				 && (parseInfo.count < parser_cardinality.minimum
					 || parser_cardinality.maximum < parseInfo.count))
				|| (parser_cardinality.is_required()
					&& (parseInfo.count < parser_cardinality.minimum)))
			{
				return parse_result::runtimeError(
					result.value(),
					"Expected: " + parseInfo.parser->get_usage_text());
			}
		}
		return result;
	}

	virtual std::unique_ptr<parser_base> clone() const override
	{
		return std::unique_ptr<parser_base>(new cli_parser(*this));
	}

	private:
	mutable exe_name m_exeName;
	std::vector<std::unique_ptr<parser_base>> m_parsers;

	void writeToStream(std::ostream& os) const
	{
		if (!m_exeName.name().empty())
		{
			os << "Usage:\n"
			   << "  " << m_exeName.name();
			for (auto const& p : m_parsers)
			{
				std::string usage_test = p->get_usage_text();
				if (usage_test.size() > 0)
				{
					os << " ";
					if (p->is_optional()) os << "[";
					os << usage_test;
					if (p->is_optional()) os << "]";
				}
			}
			os << "\n\n";
		}

		for (auto const& p : m_parsers)
		{
			auto child_description = p->get_description_text();
			if (!child_description.empty()) os << child_description << "\n\n";
		}

		os << "Options, arguments:";
		for (auto const& cols : get_help_text())
		{
			os << "\n  " << cols.option << "\n\n  " << cols.description << "\n";
		}
	}
};

/* tag::reference[]

[#lyra_cli_parser_ctor]
== Construction

end::reference[] */

/* tag::reference[]

[#lyra_cli_parser_ctor_default]
=== Default

[source]
----
cli_parser() = default;
----

Default constructing a `cli_parser` is the starting point to adding arguments
and options for parsing a command line.

end::reference[] */

/* tag::reference[]

[#lyra_cli_parser_ctor_copy]
=== Copy

[source]
----
cli_parser::cli_parser(const cli_parser& other);
----

end::reference[] */
inline cli_parser::cli_parser(const cli_parser& other)
	: m_exeName(other.m_exeName)
{
	for (auto& other_parser : other.m_parsers)
	{
		m_parsers.push_back(other_parser->clone());
	}
}

/* tag::reference[]

[#lyra_cli_parser_specification]
== Specification

end::reference[] */

// ==

/* tag::reference[]
[#lyra_cli_parser_add_argument]
=== `lyra::cli_parser::add_argument`

[source]
----
cli_parser& cli_parser::add_argument(exe_name const& exe_name);
cli_parser& cli_parser::operator|=(exe_name const& exe_name);
cli_parser& cli_parser::add_argument(parser_base const& p);
cli_parser& cli_parser::operator|=(parser_base const& p);
cli_parser& cli_parser::add_argument(cli_parser const& other);
cli_parser& cli_parser::operator|=(cli_parser const& other);
----

Adds the given argument parser to the considered arguments for this
`cli_parser`. Depending on the parser given it will be: recorded as the exe
name (for `exe_name` parser), directly added as an argument (for
`parser_base`), or add the parsers from another `cli_parser` to this one.

end::reference[] */
inline cli_parser& cli_parser::add_argument(exe_name const& exe_name)
{
	m_exeName = exe_name;
	return *this;
}
inline cli_parser& cli_parser::operator|=(exe_name const& exe_name)
{
	return this->add_argument(exe_name);
}
inline cli_parser& cli_parser::add_argument(parser_base const& p)
{
	m_parsers.push_back(p.clone());
	return *this;
}
inline cli_parser& cli_parser::operator|=(parser_base const& p)
{
	return this->add_argument(p);
}
inline cli_parser& cli_parser::add_argument(cli_parser const& other)
{
	for (auto& p : other.m_parsers)
	{
		m_parsers.push_back(p->clone());
	}
	return *this;
}
inline cli_parser& cli_parser::operator|=(cli_parser const& other)
{
	return this->add_argument(other);
}

template <typename DerivedT, typename T>
cli_parser operator|(composable_parser<DerivedT> const& thing, T const& other)
{
	return cli_parser() | static_cast<DerivedT const&>(thing) | other;
}

/* tag::reference[]
[#lyra_cli_parser_parse]
=== `lyra::cli_parser::parse`

[source]
----
parse_result cli_parser::parse(
	args const& args, parser_customization const& customize) const;
----

Parses given arguments `args` and optional parser customization `customize`.
The result indicates success or failure, and if failure what kind of failure
it was. The state of variables bound to options is unspecified and any bound
callbacks may have been called.

end::reference[] */
inline parse_result
cli_parser::parse(args const& args, parser_customization const& customize) const
{
	return parse(
		args.exe_name(),
		detail::token_iterator(
			args, customize.token_delimiters(), customize.option_prefix()),
		customize);
}

} // namespace lyra

#endif

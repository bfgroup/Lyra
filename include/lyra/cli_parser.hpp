// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_CLI_PARSER_HPP
#define LYRA_CLI_PARSER_HPP

#include "lyra/parser.hpp"
#include "lyra/exe_name.hpp"

namespace lyra
{
/* tag::reference[]

= `lyra::cli_parser`

A Combined parser made up of any two or more other parsers. Creating and using
one of these as a basis one can incrementally compose other parsers into this
one. For example:

[source]
----
auto cli = lyra::cli_parser();
std::string what;
float when = 0;
cli |= lyra::opt(what, "what")["--make-it-so"]("Make it so.").required();
cli |= lyra::opt(when. "when")["--time"]("When to do <what>.").optional();
----

*/ // end::reference[]
class cli_parser : parser_base
{
	public:

	cli_parser() = default;

	// Copy construction, needs to copy the exe name and the composed parsers.
	cli_parser(const cli_parser& other)
		: m_exeName(other.m_exeName)
	{
		for (auto & other_parser: other.m_parsers)
		{
			m_parsers.push_back(other_parser->clone());
		}
	}

	// Compose the `exe_name` parser.
	cli_parser& operator|=(exe_name const& exe_name)
	{
		m_exeName = exe_name;
		return *this;
	}

	// Compose a regular parser.
	cli_parser& operator|=(parser_base const& p)
	{
		m_parsers.push_back(p.clone());
		return *this;
	}

	// Compose the parsers from another `cli_parser`.
	cli_parser& operator|=(cli_parser const& other)
	{
		for (auto & p: other.m_parsers)
		{
			m_parsers.push_back(p->clone());
		}
		return *this;
	}

	// Concat composition.
	template <typename T>
	cli_parser operator|(T const& other) const
	{
		return cli_parser(*this) |= other;
	}

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

	friend auto operator<<(std::ostream& os, cli_parser const& parser)
		-> std::ostream&
	{
		parser.writeToStream(os);
		return os;
	}

	auto validate() const -> result override
	{
		for (auto const& p : m_parsers)
		{
			auto result = p->validate();
			if (!result)
				return result;
		}
		return result::ok();
	}

	using parser_base::parse;

	auto parse(std::string const& exe_name, detail::token_iterator const& tokens, parser_customization const& customize) const
		-> parse_result override
	{
		struct ParserInfo
		{
			parser_base const* parser = nullptr;
			size_t count = 0;
		};
		std::vector<ParserInfo> parseInfos(m_parsers.size());
		{
			size_t i = 0;
			for (auto const& p : m_parsers)
				parseInfos[i++].parser = p.get();
		}

		m_exeName.set(exe_name);

		auto result = parse_result::ok(
			detail::parse_state(parser_result_type::no_match, tokens));
		while (result.value().remainingTokens())
		{
			bool tokenParsed = false;

			for (auto & parseInfo : parseInfos)
			{
				if (parseInfo.parser->cardinality_count() == 0 || parseInfo.count < parseInfo.parser->cardinality_count())
				{
					result = parseInfo.parser->parse(
						exe_name, result.value().remainingTokens(), customize);
					if (!result)
						return result;
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
					"Unrecognized token: " + result.value().remainingTokens()->name);
		}
		// Check missing required options.
		for (auto & parseInfo : parseInfos)
		{
			if (!parseInfo.parser->is_optional() && parseInfo.count == 0)
			{
				return parse_result::runtimeError(
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
					if (p->is_optional()) os << "[" ;
					os << usage_test ;
					if (p->is_optional()) os << "]" ;
				}
			}
			os << "\n\n";
		}

		os << "Options, arguments:";
		for (auto const& cols : get_help_text())
		{
			os << "\n  " << cols.option << "\n\n  " << cols.description << "\n";
		}
	}
};

template <typename DerivedT, typename T>
cli_parser operator|(composable_parser<DerivedT> const& thing, T const& other)
{
	return cli_parser() | static_cast<DerivedT const&>(thing) | other;
}
}

#endif

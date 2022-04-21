// Copyright 2018-2022 René Ferdinand Rivera Morell
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_ARG_HPP
#define LYRA_ARG_HPP

#include "lyra/detail/print.hpp"
#include "lyra/parser.hpp"

namespace lyra {

/* tag::reference[]

[#lyra_arg]
= `lyra::arg`

A parser for regular arguments, i.e. not `--` or `-` prefixed. This is simply
a way to get values of arguments directly specified in the cli.

Is-a <<lyra_bound_parser>>.

*/ // end::reference[]
class arg : public bound_parser<arg>
{
	public:
	using bound_parser::bound_parser;

	virtual std::string get_usage_text(const option_style &) const override
	{
		std::ostringstream oss;
		if (!m_hint.empty())
		{
			auto c = cardinality();
			if (c.is_required())
			{
				for (size_t i = 0; i < c.minimum; ++i)
					oss << (i > 0 ? " " : "") << "<" << m_hint << ">";
				if (c.is_unbounded())
					oss << (c.is_required() ? " " : "") << "[<" << m_hint
						<< ">...]";
			}
			else if (c.is_unbounded())
			{
				oss << "[<" << m_hint << ">...]";
			}
			else
			{
				oss << "<" << m_hint << ">";
			}
		}
		return oss.str();
	}

	virtual help_text get_help_text(const option_style & style) const override
	{
		return { { get_usage_text(style), m_description } };
	}

	using parser::parse;

	parse_result parse(detail::token_iterator const & tokens,
		const option_style & style) const override
	{
		(void)style;
		LYRA_PRINT_SCOPE("arg::parse");
		auto validationResult = validate();
		if (!validationResult) return parse_result(validationResult);

		if (!tokens)
		{
			// Nothing to match against.
			return parse_result::ok(
				detail::parse_state(parser_result_type::no_match, tokens));
		}

		auto const & token = tokens.argument();

		auto valueRef = static_cast<detail::BoundValueRefBase *>(m_ref.get());

		if (value_choices)
		{
			auto choice_result = value_choices->contains_value(token.name);
			if (!choice_result)
			{
				LYRA_PRINT_DEBUG(
					"(!)", get_usage_text(style), "!=", token.name);
				return parse_result(choice_result);
			}
		}

		auto set_result = valueRef->setValue(token.name);
		if (!set_result)
		{
			LYRA_PRINT_DEBUG("(!)", get_usage_text(style), "!=", token.name);
			return parse_result(set_result);
		}
		else
		{
			LYRA_PRINT_DEBUG("(=)", get_usage_text(style), "==", token.name);
			auto remainingTokens = tokens;
			remainingTokens.pop(token);
			return parse_result::ok(detail::parse_state(
				parser_result_type::matched, remainingTokens));
		}
	}
};

} // namespace lyra

#endif

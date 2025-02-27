// Copyright René Ferdinand Rivera Morell
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_ARG_HPP
#define LYRA_ARG_HPP

#include "lyra/detail/bound.hpp"
#include "lyra/detail/print.hpp"
#include "lyra/detail/tokens.hpp"
#include "lyra/option_style.hpp"
#include "lyra/parser.hpp"
#include "lyra/parser_result.hpp"

#include <cstddef>
#include <string>

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

	std::string get_usage_text(const option_style &) const override
	{
		std::string text;
		if (!m_hint.empty())
		{
			auto c = cardinality();
			if (c.is_required())
			{
				for (std::size_t i = 0; i < c.minimum; ++i)
					(((text += (i > 0 ? " " : "")) += "<") += m_hint) += ">";
				if (c.is_unbounded())
					(((text += (c.is_required() ? " " : "")) += "[<") += m_hint)
						+= ">...]";
			}
			else if (c.is_unbounded())
			{
				((text += "[<") += m_hint) += ">...]";
			}
			else
			{
				((text += "<") += m_hint) += ">";
			}
		}
		return text;
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
				return parse_result::ok(
					detail::parse_state(parser_result_type::no_match, tokens));
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

	protected:
	std::string get_print_order_key(const option_style &) const override
	{
		return this->hint();
	}

	void print_help_text_details(
		printer & p, const option_style & style) const override
	{
		p.option(style, get_usage_text(style), m_description);
	}
};

} // namespace lyra

#endif

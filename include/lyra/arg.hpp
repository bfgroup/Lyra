// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_ARG_HPP
#define LYRA_ARG_HPP

#include "lyra/parser.hpp"

namespace lyra
{
// A parser for arguments
class arg : public bound_parser<arg>
{
    public:
    using bound_parser::bound_parser;

    virtual std::string get_usage_text() const override
    {
        if (!m_hint.empty())
            return "<"+m_hint+">";
        else
            return "";
    }

    virtual help_text get_help_text() const override
    {
        std::ostringstream oss;
        if (!m_hint.empty())
            oss << "<" << m_hint << ">";
        if (cardinality_count() > 1)
            oss << " ...";
        return { { oss.str(), m_description } };
    }

    auto parse(std::string const&, detail::token_iterator const& tokens, parser_customization const&) const
        -> parse_result override
    {
        auto validationResult = validate();
        if (!validationResult)
            return parse_result(validationResult);

        auto remainingTokens = tokens;
        auto const& token = *remainingTokens;
        if (token.type != detail::token_type::argument)
            return parse_result::ok(
                detail::parse_state(parser_result_type::no_match, remainingTokens));

        assert(!m_ref->isFlag());
        auto valueRef = static_cast<detail::BoundValueRefBase*>(m_ref.get());

        auto result = valueRef->setValue(remainingTokens->token);
        if (!result)
            return parse_result(result);
        else
            return parse_result::ok(
                detail::parse_state(parser_result_type::matched, ++remainingTokens));
    }

    virtual std::unique_ptr<parser_base> clone() const override
    {
        return std::unique_ptr<parser_base>(new arg(*this));
    }
};
}

#endif

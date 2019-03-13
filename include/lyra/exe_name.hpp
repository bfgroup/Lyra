// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_EXE_NAME_HPP
#define LYRA_EXE_NAME_HPP

#include "lyra/parser.hpp"
#include "lyra/parser_result.hpp"
#include "lyra/detail/bound.hpp"
#include "lyra/detail/tokens.hpp"

namespace lyra
{
// Specifies the name of the executable
class exe_name : public composable_parser<exe_name>
{
    std::shared_ptr<std::string> m_name;
    std::shared_ptr<detail::BoundValueRefBase> m_ref;

    public:
    exe_name()
        : m_name(std::make_shared<std::string>("<executable>"))
    {
    }

    explicit exe_name(std::string& ref)
        : exe_name()
    {
        m_ref = std::make_shared<detail::BoundValueRef<std::string>>(ref);
    }

    template <typename LambdaT>
    explicit exe_name(LambdaT const& lambda)
        : exe_name()
    {
        m_ref = std::make_shared<detail::BoundLambda<LambdaT>>(lambda);
    }

    // The exe name is not parsed out of the normal tokens, but is handled
    // specially
    auto parse(std::string const&, detail::token_iterator const& tokens, parser_customization const&) const
        -> parse_result override
    {
        return parse_result::ok(
            detail::parse_state(parser_result_type::no_match, tokens));
    }

    auto name() const -> std::string { return *m_name; }
    auto set(std::string const& newName) -> parser_result
    {
        auto lastSlash = newName.find_last_of("\\/");
        auto filename = (lastSlash == std::string::npos)
            ? newName
            : newName.substr(lastSlash + 1);

        *m_name = filename;
        if (m_ref)
            return m_ref->setValue(filename);
        else
            return parser_result::ok(parser_result_type::matched);
    }

    virtual std::unique_ptr<parser_base> clone() const override
    {
        return std::unique_ptr<parser_base>(new exe_name(*this));
    }
};
}

#endif

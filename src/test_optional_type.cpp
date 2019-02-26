/*
Copyright Rene Rivera 2019
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/lyra.hpp>
#include "mini_test.hpp"

int main()
{
    using namespace lyra;
    bfg::mini_test::scope test;

    #if defined(CLARA_CONFIG_OPTIONAL_TYPE)
    CLARA_CONFIG_OPTIONAL_TYPE<std::string> name;
    auto p = Opt(name, "name")
        ["-n"]["--name"]
        ("the name to use");
    {
        auto result = p.parse(Args{ "TestApp", "-q", "Pixie" });
        test
            REQUIRE( result )
            REQUIRE_FALSE( name.has_value() );
    }
    {
        auto result = p.parse(Args{ "TestApp", "-n", "Pixie" });
        test
            (REQUIRE( result ))
            (REQUIRE( name.has_value() ))
            (REQUIRE( name.value() == "Pixie" ));
    }
    #endif

    return test;
}

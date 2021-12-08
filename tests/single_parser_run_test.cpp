/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/lyra.hpp>
#include "mini_test.hpp"

int main()
{
    using namespace lyra;
    bfg::mini_test::scope test;
    std::string name;
    auto p = cli() | opt(name, "name")["-n"]["--name"]("the name to use");

    name = "";
    p.parse( { "TestApp", "-n", "Vader" } );
    test(REQUIRE(name == "Vader"));

    name = "";
    p.parse( { "TestApp", "--name", "Vader" } );
    test(REQUIRE(name == "Vader"));

    name = "";
    p.parse( { "TestApp", "-n=Vader" } );
    test(REQUIRE(name == "Vader"));

    name = "";
    p.parse( { "TestApp" } );
    test(REQUIRE(name == ""));

    name = "";
    p.parse( { "TestApp", "-f" } );
    test(REQUIRE(name == ""));

    return test;
}

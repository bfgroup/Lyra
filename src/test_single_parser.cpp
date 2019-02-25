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
    std::string name;
    auto p = Opt(name, "name")["-n"]["--name"]("the name to use");
    
    name = "";
    p.parse( Args{ "TestApp", "-n", "Vader" } );
    test(REQUIRE(name == "Vader"));

    name = "";
    p.parse( Args{ "TestApp", "--name", "Vader" } );
    test(REQUIRE(name == "Vader"));

    name = "";
    p.parse( Args{ "TestApp", "-n:Vader" } );
    test(REQUIRE(name == "Vader"));

    name = "";
    p.parse( Args{ "TestApp", "-n=Vader" } );
    test(REQUIRE(name == "Vader"));

    name = "";
    p.parse( Args{ "TestApp" } );
    test(REQUIRE(name == ""));

    name = "";
    p.parse( Args{ "TestApp", "-f" } );
    test(REQUIRE(name == ""));

    return test;
}

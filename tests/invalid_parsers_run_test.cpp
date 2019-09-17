/*
Copyright Rene Rivera 2019
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

    {
        int number = 0;
        auto cli = cli_parser() | opt( number, "number" );
        auto result = cli.parse( { "TestApp", "-o", "filename" } );
        test
            (REQUIRE( !result ))
            (REQUIRE( result.errorMessage() == "No options supplied to opt" ));
    }
    {
        int number = 0;
        auto cli = cli_parser() | opt( number, "number" )[""];
        auto result = cli.parse( { "TestApp", "-o", "filename" } );
        test
            (REQUIRE( !result ))
            (REQUIRE( result.errorMessage() == "Option name cannot be empty" ));
    }
    {
        int number = 0;
        auto cli = cli_parser() | opt( number, "number" )["invalid"];
        auto result = cli.parse( { "TestApp", "-o", "filename" } );
        test
            (REQUIRE( !result ))
            (REQUIRE( result.errorMessage().find( "Option name must begin with '-'" ) == 0 ));
    }

    return test;
}

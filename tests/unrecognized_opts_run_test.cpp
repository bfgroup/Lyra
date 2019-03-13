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
        bool a = false;
        auto cli = cli_parser() | opt( a )["-a"];
        auto result = cli.parse( { "TestApp", "-b" } );
        test
            (REQUIRE( !result ))
            (REQUIRE( result.errorMessage().find( "Unrecognized token") != std::string::npos ))
            (REQUIRE( result.errorMessage().find( "-b" ) != std::string::npos ));
    }

    return test;
}

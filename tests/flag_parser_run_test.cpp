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
    bool flag = false;
    auto p = cli_parser() | opt( flag, "true|false" )
            ["-f"]
            ("A flag");

    flag = false;
    {
        auto result = p.parse( {"TestApp", "-f", "true"} );
        test
            (REQUIRE( result ))
            (REQUIRE( flag ));
    }
    flag = false;
    {
        auto result = p.parse( {"TestApp", "-f", "yes"} );
        test
            (REQUIRE( result ))
            (REQUIRE( flag ));
    }
    flag = false;
    {
        auto result = p.parse( {"TestApp", "-f", "y"} );
        test
            (REQUIRE( result ))
            (REQUIRE( flag ));
    }
    flag = false;
    {
        auto result = p.parse( {"TestApp", "-f", "1"} );
        test
            (REQUIRE( result ))
            (REQUIRE( flag ));
    }
    flag = false;
    {
        auto result = p.parse( {"TestApp", "-f", "on"} );
        test
            (REQUIRE( result ))
            (REQUIRE( flag ));
    }
    flag = false;
    {
        auto result = p.parse( {"TestApp", "-f", "tRUe"} );
        test
            (REQUIRE( result ))
            (REQUIRE( flag ));
    }
    flag = false;
    {
        flag = true;
        auto result = p.parse( {"TestApp", "-f", "false"} );
        test
            (REQUIRE( result))
            (REQUIRE( flag == false ));
    }
    flag = false;
    {
        auto result = p.parse( {"TestApp", "-f", "what"} );
        test
            (REQUIRE( !result ))
            (REQUIRE( result.errorMessage().find( "Unable to convert" ) != std::string::npos ));
    }
    flag = false;
    {
        auto result = p.parse( {"TestApp", "-f"} );
        test
            (REQUIRE( !result ))
            (REQUIRE( result.errorMessage().find( "Expected argument following -f" ) != std::string::npos ));
    }

    return test;
}

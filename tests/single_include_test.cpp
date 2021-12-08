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
    bool show_help = false;
    struct Config
    {
        int seed = 0;
        std::string name;
        std::vector<std::string> tests;
        bool flag = false;
        double value = 0;
    } config;
    auto parser
        = help( show_help )
        | opt( config.seed, "time|value" )
            ["--rng-seed"]["-r"]
            ("set a specific seed for random numbers" )
        | opt( config.name, "name" )
            ["-n"]["--name"]
            ( "the name to use" )
        | opt( config.flag )
            ["-f"]["--flag"]
            ( "a flag to set" )
        | opt( [&]( double value ){ config.value = value; }, "number" )
            ["-d"]["--double"]
            ( "just some number" )
        | arg( config.tests, "test name|tags|pattern" )
            ( "which test or tests to use" );

    show_help = false;
    config = Config();
    {
        auto result = parser.parse( { "TestApp", "-n", "Bill", "-d=123.45", "-f", "test1", "test2" } );
        test
            (REQUIRE( result ))
            (REQUIRE( result.value().type() == parser_result_type::matched ))
            (REQUIRE( config.name == "Bill" ))
            (REQUIRE( config.value == 123.45 ))
            (REQUIRE( (config.tests == std::vector<std::string> { "test1", "test2" }) ))
            (REQUIRE( show_help == false ))
            ;
    }
    show_help = false;
    config = Config();
    {
        auto result = parser.parse( { "TestApp", "-?", "-n=NotSet" } );
        test
            (REQUIRE( result ))
            (REQUIRE( result.value().type() == parser_result_type::short_circuit_all ))
            (REQUIRE( config.name == "" )) // We should never have processed -n:NotSet
            (REQUIRE( show_help == true ))
            ;
    }

    return test;
}

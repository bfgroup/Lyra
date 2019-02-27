/*
Copyright Rene Rivera 2019
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/lyra.hpp>
#include <iostream>

int main(int argc, const char ** argv)
{
    using namespace lyra;
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
        = Help( show_help )
        | Opt( config.seed, "time|value" )
            ["--rng-seed"]["-r"]
            ("set a specific seed for random numbers" )
            .required()
        | Opt( config.name, "name" )
            ["-n"]["--name"]
            ( "the name to use" )
        | Opt( config.flag )
            ["-f"]["--flag"]
            ( "a flag to set" )
        | Opt( [&]( double value ){ config.value = value; }, "number" )
            ["-d"]["--double"]
            ( "just some number" )
        | Arg( config.tests, "test name|tags|pattern" )
            ( "which test or tests to use" );
    
    parser.parse({argc, argv});

    if (show_help)
    {
        std::cout << parser << "\n";
    }

    return 0;
}

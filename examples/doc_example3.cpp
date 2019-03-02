/*
Copyright Rene Rivera 2019
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/lyra.hpp>
#include <iostream>
#include <cstdlib>

// tag::part1[]
int main(int argc, const char ** argv)
{
    // Where we read in the argument values:
    int width = 0;
    std::string name;
    bool doIt = false;
    bool show_help = false; // <1>

    // The parser with the multiple option arguments and help option.
    auto cli
        = lyra::Help( show_help ) // <2>
        | lyra::Opt( width, "width" )
            ["-w"]["--width"]
            ("How wide should it be?")
        | lyra::Opt( name, "name" )
            ["-n"]["--name"]
            ("By what name should I be known")
        | lyra::Opt( doIt )
            ["-d"]["--doit"]
            ("Do the thing" );

    // ...
    // end::part1[]
    // tag::part2[]
    // ...

    // Parse the program arguments:
    auto result = cli.parse( { argc, argv } );

    // Check that the arguments where valid:
    if ( !result )
    {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        std::cerr << cli << "\n"; // <1>
        exit(1);
    }

    // Show the help when asked for.
    if (show_help) // <2>
    {
        std::cout << cli << "\n";
        exit(0);
    }

    std::cout << "width = " << width << ", name = " << name << ", doIt = " << doIt << "\n";
}
// end::part2[]

/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

// tag::doc[]
#include <cstdlib>
#include <iostream>
#include <lyra/lyra.hpp>

int main(int argc, const char** argv)
{
	// Where we read in the argument values:
	int width = 0;
	std::string name;
	bool doIt = false;

	// The parser with the multiple option arguments. They are composed
	// together by the "|" operator.
	auto cli = lyra::cli();
	cli.add_argument(lyra::opt(width, "width")
		.name("-w").name("--width").help("How wide should it be?"));
	cli.add_argument(lyra::opt(name, "name")
		.name("-n").name("--name").help("By what name should I be known"));
	cli.add_argument(lyra::opt(doIt)
		.name("-d").name("--doit").help("Do the thing"));

	// Parse the program arguments:
	auto result = cli.parse({ argc, argv });

	// Check that the arguments where valid:
	if (!result)
	{
		std::cerr << "Error in command line: " << result.message() << std::endl;
		return 1;
	}

	std::cout << "width = " << width << ", name = " << name << ", doIt = " << doIt << "\n";
	return 0;
}
// end::doc[]

/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

// tag::part1[]
#include <iostream>
#include <lyra/lyra.hpp>

int main(int argc, const char** argv)
{
	int choice = 5;
	// Ex: <exe> --choice=3
	auto cli = lyra::cli()
		| lyra::opt(choice, "-c")["--choice"]
			  .choices([](int value) { return 1 <= value && value <= 10; });
	auto result = cli.parse({ argc, argv });
	if (result)
	{
		std::cout << "Your number between one and ten is " << choice << "\n";
		return 0;
	}
	else
	{
		std::cerr << result.message() << "\n";
		return 1;
	}
}
// end::part1[]

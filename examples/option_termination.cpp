/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/* tag::doc[]

= Tail Arguments

A common pattern for command line programs is to use "escaped" arguments at the
end of the command. Usually with an empty option prefix as the escape indicator.
For example: `command --option -- arbitrary arguments --more`. For which the
`--option` is treated as a regular option. But everything after the `--` is
immune to parsing for options. Such behaviour can be done in Lyra with a
combination of a group, a literal, and a container to capture the rest of the
arguments.

This example shows how you could do that. Do note that as one can create
multiple Lyra parsers. It would also be possible to use an additional parser to
also parse the escaped argument differently.

[source]
----
end::doc[] */

// tag::doc[]
#include <iostream>
#include <lyra/lyra.hpp>
#include <string>
#include <vector>

int main(int argc, const char ** argv)
{
	bool echo = false;
	bool help = false;
	std::vector<std::string> rest;

	auto cli = lyra::cli();
	cli.add_argument(lyra::help(help));
	cli.add_argument(lyra::opt(echo).name("--echo").help(
		"Echo the post options arguments."));
	cli.add_argument(lyra::group()
			.add_argument(lyra::literal("--"))
			.add_argument(lyra::arg(rest, "rest")));

	auto result = cli.parse({ argc, argv });

	if (!result)
	{
		std::cerr << "Error in command line: " << result.message() << "\n";
		return 1;
	}

	if (help)
	{
		std::cout << cli << "\n";
		return 0;
	}

	if (echo)
	{
		std::cout << "rest:";
		for (auto & r : rest) std::cout << " " << r;
		std::cout << "\n";
	}
	return 0;
}
// end::doc[]
/* tag::doc[]
----
end::doc[] */

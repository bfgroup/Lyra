/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/* tag::doc[]

= Main

For many cases specifying a help option, handling it, and specifying additional
arguments introduces boilerplate that can get in the way of managing "easy"
command lines. The `lyra::main` class abstract most of that boilerplate to make
it trivial to create and handle a command line.

In this example we define and use two options and one positional argument. It
then executes the lambda with the parsed command line as given by `argv`.

[source]
----
end::doc[] */

// tag::doc[]
#include <iostream>
#include <lyra/lyra.hpp>

int main(int argc, const char ** argv)
{
	return lyra::main() // <1>
		("-x", 0)("-y", 0)("z", 5) // <2>
		(argc, argv, [](lyra::main & m) { // <3>
			std::cout << (int(m["-x"]) + int(m["-y"])) * int(m["z"]) << "\n"; // <4>
			return 0; // <5>
		});
}
// end::doc[]
/* tag::doc[]
----
<1> Use the `lyra::main` utility by creating an instance of it.
<2> Define two options and one positional argument. Each one defines the one
	option, or hint, and the default value (and type of value).
<3> Define and execute the body of the program passed in as a lambda.
<4> Access the individual options and argument from any of their names or hint.
	Each is converted to the specified type (`int` in this case).
<5> Return an appropriate value that is also return from `main` entry function.
end::doc[] */

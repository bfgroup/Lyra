/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/


/* tag::doc[]

= More Main

Using the simple `lyra::main` interface can't possibly cover the variety of use
cases people want. In particular when it comes to the options and arguments.
For that teh `lyra::main` interface supports adding instances of the regular
option and argument parsers.

In this example we define and use two options and one positional argument, as
before. But we use `lyra::opt` and `lyra::arg` specifications. We also use
the `lyra::val` value holder to provide the location for the parsed value
and it's parsed type.

[source]
----
end::doc[] */

// tag::doc[]
#include <iostream>
#include <lyra/lyra.hpp>

int main(int argc, const char ** argv)
{
	return lyra::main()
		(lyra::opt(lyra::val(0), "x")["-x"]) // <1>
		(lyra::opt(lyra::val(0), "y")["-y"])
		(lyra::arg(lyra::val(5), "z")) // <2>
		(argc, argv, [](lyra::main & m)
		{
			std::cout << (int(m["-x"]) + int(m["-y"]))*int(m["z"]) << "\n";
			return 0;
		});
}
// end::doc[]
/* tag::doc[]
----
<1> Add the "-x" option as an integer with a default of `0`.
<2> The positional argument in this case has a default of `5`.
end::doc[] */

/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/


/* tag::doc[]

= Counting Flags

Some times we want to allow multiple flags to be specified. This is most common
with a verbose (`-v`) option. For example in passing in `-vvv` to indicate more
verbose output from a program than using `-v`. To do this we can specify the
cardinality as usual, and make use of passing in a lambda for the value of the
option instead of a variable reference. With the lambda we can count the number
of the flag occurrences.

[source]
----
end::doc[] */

// tag::doc[]
#include <iostream>
#include <lyra/lyra.hpp>

int main(int argc, const char ** argv)
{
	int verbose = 0; // <1>
	return lyra::main()
		(lyra::opt(
			[&](bool){ verbose += 1; }) // <2>
			["-v"].cardinality(0,5)) // <3>
		(argc, argv, [&](lyra::main &)
		{
			std::cout << verbose << "\n"; // <4>
			return 0;
		});
}
// end::doc[]
/* tag::doc[]
----
<1> We start off the verbose as "off" by having zero count.
<2> The option takes a lambda for the value that counts the flag each time
the user specifies it.
<3> We allow a verbose level up to 5, i.e. five `-v` flags can be specified.
<4> We end up with the counted `verbose` level after parsing.
end::doc[] */

/*
Copyright 2021-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/* tag::doc[]

= Option Styles

This library, by default, uses the style of specifying options most commonly
seen in POSIX flavored systems like Linux and BSD of having long options
prefixed with double dash (`--`) where the value follow an equal (`=`). And
short options, i.e. flags, prefixed with a single dash (`-`). This is not, by
far, the only variation of option style in the wild. To accommodate such
style variations one can use the <<lyra_option_style>> class to define what
one wants the user to use. The <<lyra_option_style>> is independent of the
syntax used to specify the options for parsing. And hence the same argument
option definition can be used to parse different option styles.

There are three aspects of the options that can be configured in the
<<lyra_option_style>>:

`value_delimiters`:: The characters between an option and its value.
`long_option_prefix` and `long_option_size`:: The characters for long option
	name prefix, and how many repetitions of the character.
`short_option_prefix` and `short_option_size`:: The characters for short
	option name prefix, and how many repetitions of the character.

To specify the default POSIX style one would specify it with these settings:

[horizontal]
`value_delimiters`:: `=` and space
`long_option_prefix`:: `-`
`long_option_size`:: `2`
`short_option_prefix`:: `-`
`short_option_size`:: `1`

There are some built-in option <<lyra_option_style_styles>> defined for common
cases returned by class methods. But you can define, and use your own using the
the <<lyra_cli_style>> method. For example:

[source]
----
end::doc[] */

// tag::doc[]
#include <iostream>
#include <lyra/lyra.hpp>

int main(int argc, const char ** argv)
{
	using namespace lyra;

	bool show_help = false;
	std::string name;
	bool flag = false;
	auto cli
		= help(show_help).description("This is a sample CLI parser.") // <1>
		| opt(name, "name")["-n"]["--name"]("The name to use.") // <2>
		| opt(flag)["-f"]["--flag"]("A flag to set.");

	cli.style({ ",", "+", 2, "~", 1 }); // <3>

	auto result = cli.parse({ argc, argv });
	if (!cli.parse({ argc, argv }))
	{
		std::cerr << result.message() << "\n\n";
	}
	if (show_help || !result)
	{
		std::cout << cli << "\n";
	}
	return result ? 0 : 1;
}
// end::doc[]

/* tag::doc[]
----
<1> The help option will add `-h`, `-?`, and `--help`.
<2> We add other options, `-n` and `--name`.
<3> Our style accepts arguments like: `++name,Jane`, `~f`, and `~?`.
end::doc[] */

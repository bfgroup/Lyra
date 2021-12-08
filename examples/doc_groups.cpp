/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

/* tag::doc[]

= Argument Groups

There are times when some CLI arguments only make sense when considered as a
collection. A common case for this are commands. But it also comes up in any
case where some arguments are all required, but only if one of them is present.
Lets take the example of specifying the dimension of a window. For example one
could expect both `--width=X` and `--height=Y` options. But you could make them
optional, as a group, in favor of an automatic screen sized window of a
particular aspect ratio with a `--aspect=R` option. In such a case we would
want to allow either one of these to be specified:

----
exec --width=1440 --height=720
exec --aspect=1.78
----

With argument groups we can allow such an arrangement.

[source]
----
*/ // end::doc[]

// tag::doc[]
#include <lyra/lyra.hpp>

#include <iostream>
#include <string>
#include <vector>

int main(int argc, const char ** argv)
{
	// Default to showing a full screen 4/3 aspect
	bool show_full_screen = true;
	float aspect = 4.0f / 3.0f;

	// If it's not full screen the size will be specified.
	unsigned width = 0;
	unsigned height = 0;

	// Did the user ask for help?
	bool get_help = false;

	lyra::cli cli;
	cli.add_argument(lyra::help(get_help))
		.add_argument(lyra::opt(aspect, "aspect") // <1>
						  .name("--aspect")
						  .help("Full-screen aspect ratio window."))
		.add_argument(lyra::group([&](const lyra::group &) {
						  show_full_screen = false;
					  }) // <2>
						  .add_argument(lyra::opt(width, "width") // <3>
											.required()
											.name("--width")
											.help("Width of window."))
						  .add_argument(lyra::opt(height, "height")
											.required() // <4>
											.name("--height")
											.help("Height of window.")));

	// Parse the program arguments.
	auto result = cli.parse({ argc, argv });

	if (get_help)
	{
		std::cout << cli;
		return 0;
	}

	// Check that the arguments where valid.
	if (!result)
	{
		std::cerr << "Error in command line: " << result.message()
				  << std::endl;
		return 1;
	}

	if (show_full_screen)
	{
		std::cout << "Full screen aspect = " << aspect << "\n";
	}
	else
	{
		std::cout << "Window size = " << width << " x " << height << "\n";
	}

	return 0;
}
// end::doc[]
/* tag::doc[]
----
<1> The `ratio` option is a regular value argument.
<2> The group for the window size turns off the full screen mode when the size
	options are fully specified.
<3> We add the `width` and `height` options to the group.
<4> We specify both the `width` and `height` as required to ensure we get only
	a valid specification when both are present. Otherwise the group is
	ignored as optional.
*/ // end::doc[]

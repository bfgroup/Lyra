/*
Copyright 2020 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/lyra.hpp>
#include "mini_test.hpp"
#include <string>
#include <sstream>

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	{
		bool a = false;
		bool show_help = false;
		auto cli = lyra::cli() | opt( a )["-a"] | help(show_help);
		std::ostringstream help_text;
		auto result = cli.parse( { "TestApp", "-?" } );
		help_text << cli;
		test
			(REQUIRE( result ))
			(REQUIRE( help_text.str().find("USAGE:") != std::string::npos ));
	}
	{
		bool a = false;
		bool show_help = false;
		auto cli = lyra::cli() | opt( a )["-a"] | help(show_help);
		std::ostringstream help_text;
		auto result = cli.parse( { "", "-?" } );
		help_text << cli;
		test
			(REQUIRE( result ))
			(REQUIRE( help_text.str().find("USAGE:") == std::string::npos ));
	}

	return test;
}

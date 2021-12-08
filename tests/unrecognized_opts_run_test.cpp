/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/lyra.hpp>
#include "mini_test.hpp"
#include <string>

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	{
		bool a = false;
		auto cli = lyra::cli() | opt( a )["-a"];
		auto result = cli.parse( { "TestApp", "-b" } );
		test
			(REQUIRE( !result ))
			(REQUIRE( result.message().find( "Unrecognized token") != std::string::npos ))
			(REQUIRE( result.message().find( "-b" ) != std::string::npos ));
	}
	#if 0 // Test case for https://github.com/bfgroup/Lyra/issues/24
	{
		bool o = false;
		std::string a;
		auto cli = lyra::cli()
			.add_argument(opt( o )["--option"].optional())
			.add_argument(arg(a, "argument"));
		auto result = cli.parse( { "TestApp", "--function", "arg" } );
		test
			(REQUIRE( !result ))
			(REQUIRE( result.message().find( "Unrecognized token") != std::string::npos ))
			(REQUIRE( result.message().find( "--function" ) != std::string::npos ));
	}
	#endif

	return test;
}

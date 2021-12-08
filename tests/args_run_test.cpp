/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/lyra.hpp>
#include "mini_test.hpp"

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	std::string value;
	auto cli = lyra::cli() | arg( value, "value" );

	value = "";
	{
		char* exec_args[] = { (char*)"TestApp", (char*)"hello" };
		auto result = cli.parse( { 2, exec_args } );
		test
			(REQUIRE( result ))
			(REQUIRE( value == "hello" ));
	}
	value = "";
	{
		const char* exec_args[] = { "TestApp", "hello" };
		auto result = cli.parse( { 2, exec_args } );
		test
			(REQUIRE( result ))
			(REQUIRE( value == "hello" ));
	}
	value = "";
	{
		std::vector<const char*> exec_args { "TestApp", "hello" };
		auto result = cli.parse( args( exec_args.begin(), exec_args.end() ) );
		test
			(REQUIRE( result ))
			(REQUIRE( value == "hello" ));
	}
	value = "";
	{
		std::vector<const char*> exec_args { "TestApp", "hello" };
		auto result = cli.parse( { exec_args.begin(), exec_args.end() } );
		test
			(REQUIRE( result ))
			(REQUIRE( value == "hello" ));
	}

	return test;
}

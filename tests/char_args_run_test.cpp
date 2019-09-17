/*
Copyright Rene Rivera 2019
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
	auto cli = cli_parser() | arg( value, "value" );

	value = "";
	{
		char* args[] = { (char*)"TestApp", (char*)"hello" };
		auto result = cli.parse( { 2, args } );
		test
			(REQUIRE( result ))
			(REQUIRE( value == "hello" ));
	}
	value = "";
	{
		const char* args[] = { "TestApp", "hello" };
		auto result = cli.parse( { 2, args } );
		test
			(REQUIRE( result ))
			(REQUIRE( value == "hello" ));
	}

	return test;
}

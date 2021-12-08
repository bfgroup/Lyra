/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	{
		int number = 0;
		auto cli = lyra::cli() | opt(number, "number");
		auto result = cli.parse({ "TestApp", "-o", "filename" });
		test(REQUIRE(!result));
		test(REQUIRE(result.message() == "No options supplied to opt"));
	}
	{
		int number = 0;
		auto cli = lyra::cli() | opt(number, "number")[""];
		auto result = cli.parse({ "TestApp", "-o", "filename" });
		test(REQUIRE(!result));
		test(REQUIRE(result.message() == "Option name cannot be empty"));
	}
	{
		int number = 0;
		auto cli = lyra::cli() | opt(number, "number")["invalid"];
		auto result = cli.parse({ "TestApp", "-o", "filename" });
		test(REQUIRE(!result));
		test(REQUIRE(
			result.message().find("Option name must begin with '-'")
			== 0));
	}

	return test;
}

/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/lyra.hpp>
#include <vector>
#include "mini_test.hpp"

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	{
		int number = 1;
		auto cli = lyra::cli()
				| opt(number, "number")["--number"];
		auto result = cli.parse({ "TestApp", "--number", "42" });
		test
			(REQUIRE(result))
			(REQUIRE(number == 42));
	}
	{
		std::vector<int> numbers;
		auto cli = lyra::cli() |
				opt(numbers, "number")["--number"].cardinality(0);
		auto result = cli.parse({ "TestApp", "--number", "42", "--number", "33" });
		test
			(REQUIRE(result))
			(REQUIRE(numbers[0] == 42))
			(REQUIRE(numbers[1] == 33));
	}
	{
		std::vector<int> numbers;
		auto cli = lyra::cli() |
				opt(numbers, "number")["--number"].cardinality(0, 2);
		{
			auto result = cli.parse({ "TestApp" });
			test
				(REQUIRE(result))
				(REQUIRE(numbers.size() == 0));
		}
		{
			auto result = cli.parse({ "TestApp", "--number", "42", "--number", "33" });
			test
				(REQUIRE(result))
				(REQUIRE(numbers[0] == 42))
				(REQUIRE(numbers[1] == 33));
		}
		{
			auto result = cli.parse({ "TestApp", "--number", "42", "--number", "33", "--number", "1" });
			test
				(REQUIRE(!result));
		}
	}
	{
		std::vector<int> numbers;
		auto cli = lyra::cli() |
				opt(numbers, "number")["--number"].cardinality(2, 3);
		{
			auto result = cli.parse({ "TestApp", "--number", "42", "--number", "33", "--number", "1" });
			test
				(REQUIRE(result));
		}
		{
			auto result = cli.parse({ "TestApp", "--number", "42" });
			test
				(REQUIRE(!result));
		}
	}
	{
		std::vector<int> numbers;
		auto cli = lyra::cli() |
				arg(numbers, "number").required();
		{
			auto result = cli.parse({ "TestApp", "42", "33", "1" });
			test
				(REQUIRE(result));
		}
		{
			auto result = cli.parse({ "TestApp" });
			test
				(REQUIRE(!result));
		}
	}

	return test;
}

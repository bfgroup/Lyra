/*
Copyright 2021-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

#include <iostream>
#include <string>
#include <cstdint>

int main()
{
	bfg::mini_test::scope test;

	std::int32_t i32 = 7;
	std::int64_t i64 = 7;
	std::uint32_t u32 = 7;
	std::uint64_t u64 = 7;

	lyra::cli cli;
	cli.add_argument(lyra::opt(i32, "i32").name("--i32"));
	cli.add_argument(lyra::opt(i64, "i64").name("--i64"));
	cli.add_argument(lyra::opt(u32, "u32").name("--u32"));
	cli.add_argument(lyra::opt(u64, "u64").name("--u64"));

	{
		i32 = 7;
		auto result = cli.parse(
			{ "test_app", "--i32", "-1" });
		test(REQUIRE(result));
		test(REQUIRE(i32 == -1));
	}
	{
		u32 = 7;
		auto result = cli.parse(
			{ "test_app", "--u32", "-1" });
		test(REQUIRE(!result));
		test(REQUIRE(u32 == 7));
	}
	{
		i64 = 7;
		auto result = cli.parse(
			{ "test_app", "--i64", "-1" });
		test(REQUIRE(result));
		test(REQUIRE(i64 == -1));
	}
	{
		u64 = 7;
		auto result = cli.parse(
			{ "test_app", "--u64", "-1" });
		test(REQUIRE(!result));
		test(REQUIRE(u64 == 7));
	}

	return test;
}

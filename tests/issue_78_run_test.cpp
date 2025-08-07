/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

int main()
{
	bfg::mini_test::scope test;
	bool flag_this {};
	bool flag_that {};
	bool flag_extra {};
	{
		auto cli = lyra::cli();
		cli.add_argument(lyra::opt(flag_this)["--flag-this"]);
		cli.add_argument(lyra::opt(flag_that)["--flag-that"]);
		cli.add_argument(lyra::opt(flag_extra)["--flag-extra"]);
		{
			flag_this = flag_that = flag_extra = false;
			auto result
				= cli.parse({ "test_app", "--flag-this", "--flag-extra" });
			test(REQUIRE(result));
			test(REQUIRE(flag_this));
			test(REQUIRE(flag_extra));
		}
		{
			flag_this = flag_that = flag_extra = false;
			auto result
				= cli.parse({ "test_app", "--flag-extra", "--flag-this" });
			test(REQUIRE(result));
			test(REQUIRE(flag_this));
			test(REQUIRE(flag_extra));
		}
	}
	{
		auto cli = lyra::cli();
		cli.add_argument(lyra::group() | lyra::opt(flag_this)["--flag-this"]
			| lyra::opt(flag_that)["--flag-that"]);
		cli.add_argument(lyra::opt(flag_extra)["--flag-extra"]);
		{
			flag_this = flag_that = flag_extra = false;
			auto result
				= cli.parse({ "test_app", "--flag-this", "--flag-extra" });
			test(REQUIRE(result));
			test(REQUIRE(flag_this));
			test(REQUIRE(flag_extra));
		}
		{
			auto result
				= cli.parse({ "test_app", "--flag-extra", "--flag-this" });
			test(REQUIRE(result));
			test(REQUIRE(flag_this));
			test(REQUIRE(flag_extra));
		}
	}
	{
		auto cli = lyra::cli().relaxed();
		cli.add_argument(lyra::group() | lyra::opt(flag_this)["--flag-this"]
			| lyra::opt(flag_that)["--flag-that"]);
		cli.add_argument(lyra::opt(flag_extra)["--flag-extra"]);
		{
			flag_this = flag_that = flag_extra = false;
			auto result
				= cli.parse({ "test_app", "--flag-this", "--flag-extra" });
			test(REQUIRE(result));
			test(REQUIRE(flag_this));
			test(REQUIRE(flag_extra));
		}
		{
			flag_this = flag_that = flag_extra = false;
			auto result
				= cli.parse({ "test_app", "--flag-extra", "--flag-this" });
			test(REQUIRE(result));
			test(REQUIRE(flag_this));
			test(REQUIRE(flag_extra));
		}
	}
	return test;
}

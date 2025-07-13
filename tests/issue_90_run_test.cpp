/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <array>
#include <lyra/lyra.hpp>
#include <string>

int main()
{
	bfg::mini_test::scope test;
	{
		auto cli = lyra::cli {};
		auto choice = std::string {};
		cli.add_argument(lyra::opt(choice, "your choice")
				.name("--choice")
				.choices(std::string { "foo" })
				.required());
		{
			choice = "";
			auto result = cli.parse({ "test_app", "--choice", "foo" });
			test(REQUIRE(result));
			test(REQUIRE(choice == "foo"));
		}
		{
			choice = "";
			auto result = cli.parse({ "test_app", "--choice=foo" });
			test(REQUIRE(result));
			test(REQUIRE(choice == "foo"));
		}
	}
	{
		auto cli = lyra::cli {};
		auto choice = int {};
		int choices[] = { 1, 13 };
		cli.add_argument(lyra::opt(choice, "your choice")
				.name("--choice")
				.choices(choices)
				.required());
		{
			choice = 1;
			auto result = cli.parse({ "test_app", "--choice", "13" });
			test(REQUIRE(result));
			test(REQUIRE(choice == 13));
		}
		{
			choice = 1;
			auto result = cli.parse({ "test_app", "--choice=13" });
			test(REQUIRE(result));
			test(REQUIRE(choice == 13));
		}
	}
	{
		auto cli = lyra::cli {};
		auto choice = std::string {};
		cli.add_argument(lyra::opt(choice, "your choice")
				.name("--choice")
				.choices("foo")
				.required());
		{
			choice = "";
			auto result = cli.parse({ "test_app", "--choice", "foo" });
			test(REQUIRE(result));
			test(REQUIRE(choice == "foo"));
		}
		{
			choice = "";
			auto result = cli.parse({ "test_app", "--choice=foo" });
			test(REQUIRE(result));
			test(REQUIRE(choice == "foo"));
		}
	}
	{
		auto cli = lyra::cli {};
		auto choice = std::string {};
		cli.add_argument(lyra::opt(choice, "your choice")
				.name("--choice")
				.choices("foo", "fun")
				.required());
		{
			choice = "";
			auto result = cli.parse({ "test_app", "--choice", "foo" });
			test(REQUIRE(result));
			test(REQUIRE(choice == "foo"));
		}
		{
			choice = "";
			auto result = cli.parse({ "test_app", "--choice=foo" });
			test(REQUIRE(result));
			test(REQUIRE(choice == "foo"));
		}
		{
			choice = "";
			auto result = cli.parse({ "test_app", "--choice=fun" });
			test(REQUIRE(result));
			test(REQUIRE(choice == "fun"));
		}
		{
			choice = "";
			auto result = cli.parse({ "test_app", "--choice=sad" });
			test(REQUIRE(!result));
		}
	}
	return test;
}

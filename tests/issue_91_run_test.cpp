/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

#include <string>
#include <vector>

int main()
{
	bfg::mini_test::scope test;

	std::vector<std::string> cmds;
	std::string c0;
	std::string c1;
	{
		auto cli = lyra::cli();
		cli.add_argument(lyra::opt(c0, "c0")["--c0"]);
		cli.add_argument(lyra::opt(c1, "c1")["--c1"]);
		cli.add_argument(lyra::command(
			"foo", [&](lyra::group const &) { cmds.push_back("foo"); }));
		cli.add_argument(lyra::command(
			"bar", [&](lyra::group const &) { cmds.push_back("bar"); }));
		{
			cmds.clear();
			auto result = cli.parse({ "test_app", "--c1=1", "foo" });
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 1));
			test(REQUIRE(c1 == "1"));
		}
		{
			cmds.clear();
			auto result = cli.parse({ "test_app", "foo" });
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 1));
		}
		{
			cmds.clear();
			auto result = cli.parse({ "test_app", "bar" });
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 1));
		}
		{
			cmds.clear();
			auto result = cli.parse({ "test_app", "foo", "bar" });
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 2));
		}
		{
			cmds.clear();
			auto result
				= cli.parse({ "test_app", "--c1=1", "bar", "foo", "--c0=0" });
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 2));
		}
	}
	{
		auto cli = lyra::cli();
		cli.add_argument(lyra::opt(c0, "c0")["--c0"]);
		cli.add_argument(lyra::opt(c1, "c1")["--c1"]);
		cli.add_argument(lyra::group()
				.require(0, 1)
				.add_argument(lyra::command(
					"foo", [&](lyra::group const &) { cmds.push_back("foo"); }))
				.add_argument(lyra::command("bar",
					[&](lyra::group const &) { cmds.push_back("bar"); })));
		{
			cmds.clear();
			auto result = cli.parse({ "test_app", "foo", "bar" });
			test(REQUIRE(!result));
			test(REQUIRE(cmds.size() != 2));
		}
		{
			cmds.clear();
			c0 = "";
			c1 = "";
			auto result = cli.parse({ "test_app", "--c1=1", "foo" });
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 1));
			test(REQUIRE(c1 == "1"));
		}
		{
			cmds.clear();
			auto result = cli.parse({ "test_app", "foo" });
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 1));
		}
		{
			cmds.clear();
			auto result = cli.parse({ "test_app", "bar" });
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 1));
		}
		{
			cmds.clear();
			c0 = "";
			c1 = "";
			auto result
				= cli.parse({ "test_app", "--c1=1", "bar", "foo", "--c0=0" });
			test(REQUIRE(!result));
			test(REQUIRE(cmds.size() != 2));
		}
	}
	{
		auto cli = lyra::cli();
		cli.add_argument(lyra::opt(c0, "c0")["--c0"]);
		cli.add_argument(lyra::opt(c1, "c1")["--c1"]);
		cli.add_argument(lyra::command("foo", [&](lyra::group const &) {
			cmds.push_back("foo");
		}).add_argument(lyra::command("bar", [&](lyra::group const &) {
			cmds.push_back("bar");
		})));
		{
			cmds.clear();
			c0 = "";
			c1 = "";
			auto result = cli.parse({ "test_app", "--c1=1", "foo" });
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 1));
			test(REQUIRE(c1 == "1"));
		}
		{
			cmds.clear();
			auto result = cli.parse({ "test_app", "foo" });
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 1));
		}
		{
			cmds.clear();
			auto result = cli.parse({ "test_app", "bar" });
			test(REQUIRE(!result));
			test(REQUIRE(cmds.size() == 0));
		}
		{
			cmds.clear();
			auto result = cli.parse({ "test_app", "foo", "bar" });
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 2));
		}
		{
			cmds.clear();
			c0 = "";
			c1 = "";
			auto result
				= cli.parse({ "test_app", "--c1=1", "bar", "foo", "--c0=0" });
			test(REQUIRE(!result));
			test(REQUIRE(cmds.size() == 0));
		}
	}
	return test;
}

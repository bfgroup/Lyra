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
	std::string a;
	int b = 0;
	std::string c;
	{
		auto cli = lyra::cli();
		lyra::group sub;
		sub.require(1, 1);
		sub.add_argument(lyra::command(
			"foo", [&](lyra::group const &) { cmds.emplace_back("foo"); })
				.add_argument(lyra::opt(a, "foo_a").name("--foo_a"))
				.add_argument(lyra::arg(c, "foo_c").required()));
		sub.add_argument(lyra::command(
			"bar", [&](lyra::group const &) { cmds.emplace_back("bar"); })
				.add_argument(lyra::opt(b, "bar_b").name("--bar_b"))
				.add_argument(lyra::arg(c, "bar_c").required()));
		cli.add_argument(sub);
		{
			cmds.clear();
			a = "";
			b = 0;
			c = "";
			auto result = cli.parse({ "test_app", "foo" });
			test.check_result(result);
			test(REQUIRE(!result));
			test(REQUIRE(cmds.size() == 0));
			test(REQUIRE(COND_HAS_STRING(result.message(), "Expected:")
				&& COND_HAS_STRING(result.message(), "<foo_c>")));
		}
		{
			cmds.clear();
			a = "";
			b = 0;
			c = "";
			auto result = cli.parse({ "test_app", "bar" });
			test.check_result(result);
			test(REQUIRE(!result));
			test(REQUIRE(cmds.size() == 0));
			test(REQUIRE(COND_HAS_STRING(result.message(), "Expected:")
				&& COND_HAS_STRING(result.message(), "<bar_c>")));
		}
		{
			cmds.clear();
			a = "";
			b = 0;
			c = "";
			auto result = cli.parse({ "test_app" });
			test.check_result(result);
			test(REQUIRE(result));
			test(REQUIRE(cmds.size() == 0));
			test(REQUIRE(result.message().empty()));
		}
	}
	return test;
}

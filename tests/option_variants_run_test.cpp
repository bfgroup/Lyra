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
	bool a = false, b = false, c = false;
	int x = 10;
	float y = 3.14f;
	bool sdf = false;
	auto cli = opt(a)["-a"]["--flag-a"] | opt(b)["-b"]["--flag-b"]
		| opt(c)["-c"]["--flag-c"] | opt(x, "int-x")["-x"]["--int-x"]
		| opt(y, "float-y")["-y"]["--float-y"]
		| opt(sdf)["-sdf"];

	a = false, b = false, c = false;
	test(REQUIRE(
		cli.parse({ "TestApp", "-a", "-b", "-c", "-x", "3", "-y", "6.6" })));
	test(REQUIRE(a))(REQUIRE(b))(REQUIRE(c));
	a = false, b = false, c = false;
	test(REQUIRE(cli.parse({ "TestApp", "-abc", "-x", "3", "-y", "6.6" })));
	test(REQUIRE(a))(REQUIRE(b))(REQUIRE(c));
	x = 0;
	test(REQUIRE(cli.parse({ "TestApp", "-x", "3" })));
	test(REQUIRE(x == 3));
	x = 0;
	test(REQUIRE(cli.parse({ "TestApp", "-x=3" })));
	test(REQUIRE(x == 3));
	x = 0;
	test(REQUIRE(cli.parse({ "TestApp", "-x3" })));
	test(REQUIRE(x == 3));
	x = 0;
	test(REQUIRE(cli.parse({ "TestApp", "--int-x", "3" })));
	test(REQUIRE(x == 3));
	x = 0;
	test(REQUIRE(cli.parse({ "TestApp", "--int-x=3" })));
	test(REQUIRE(x == 3));
	x = 0;
	test(REQUIRE(!cli.parse({ "TestApp", "--int-x3" })));
	test(REQUIRE(x == 0));
	x = 0;
	test(REQUIRE(!cli.parse({ "TestApp", "-x3garbage" })));
	test(REQUIRE(x == 0));
	x = 0;
	test(REQUIRE(!cli.parse({ "TestApp", "-x=3garbage" })));
	test(REQUIRE(x == 0));
	x = 0;
	test(REQUIRE(!cli.parse({ "TestApp", "-x", "3garbage" })));
	test(REQUIRE(x == 0));
	test(REQUIRE(cli.parse({ "TestApp", "-s" })));
	test(REQUIRE(cli.parse({ "TestApp", "-d" })));
	test(REQUIRE(cli.parse({ "TestApp", "-f" })));

	return test;
}

/*
Copyright 2020 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

int main(int argc, const char ** argv)
{
	using namespace lyra;
	bfg::mini_test::scope test;

	{
		auto cli = lyra::cli()
			| command("one")
			| command("two");
		test
			(REQUIRE(cli.parse({ "TestApp", "one" })))
			(REQUIRE(cli.parse({ "TestApp", "two" })))
			(REQUIRE(cli.parse({ "TestApp" })));
	}
	{
		std::string one = "one";
		int two = 2;
		auto cli = lyra::cli()
			| command("one").add_argument(arg(one, "one"))
			| command("two").add_argument(arg(two, "two"));
		test
			(REQUIRE(cli.parse({ "TestApp", "one" })))
			(REQUIRE(cli.parse({ "TestApp", "two" })))
			(REQUIRE(cli.parse({ "TestApp", "one", "ONE" })))
			(REQUIRE(cli.parse({ "TestApp", "two", "3" })))
			(REQUIRE(!cli.parse({ "TestApp", "one", "OnE", "TwO" })))
			(REQUIRE(!cli.parse({ "TestApp", "two", "3", "4" })))
			(REQUIRE(cli.parse({ "TestApp" })));
	}
	{
		bool a = false;
		bool b = false;
		bool c = false;
		bool d = false;
		auto cli = lyra::cli()
			| command("one")
				.add_argument(opt(a, "a").name("--a").optional())
				.add_argument(opt(b, "b").name("--b").optional())
			| command("two")
				.add_argument(opt(c, "c").name("--c").optional())
				.add_argument(opt(d, "d").name("--d").optional())
			;
		test
			(REQUIRE(cli.parse({ "TestApp", "one", "--a=1" })))
			(REQUIRE(cli.parse({ "TestApp", "two", "--c=1" })))
			(REQUIRE(cli.parse({ "TestApp" })));
	}

	return test;
}

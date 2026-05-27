/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

#include <iostream>
#include <string>

int main()
{
	bfg::mini_test::scope test;

	{
		std::string a;
		std::string b;
		auto cli = lyra::cli();
		cli.add_argument(lyra::opt(a, "A")["-a"]["--aflag"].required());
		cli.add_argument(lyra::opt(b, "B")["-b"]["--bflag"].required());
		{
			a = "";
			b = "";
			auto result = cli.parse({ "test_app", "-a", "aye" });
			test(REQUIRE(!result));
			if (!result)
			{
				auto m = result.message();
				std::cerr << m << "\n";
				test(REQUIRE(m.find("Expected") != std::string::npos));
			}
		}
	}
	return test;
}

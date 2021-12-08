/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>
#include <vector>

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	{
		lyra::main()("-o", -1)(
			args { "main", "-o", "5" },
			[&](lyra::main & m)
			{
				test(REQUIRE(int(m["-o"]) == 5));
				return 0;
			});
	}
	{
		lyra::main()("-o", -1)(
			args { "main" },
			[&](lyra::main & m)
			{
				test(REQUIRE(int(m["-o"]) == -1));
				return 0;
			});
	}
	{
		lyra::main()("-o", -1.0f)(
			args { "main", "-o", "5.0" },
			[&](lyra::main & m)
			{
				test(REQUIRE(int(m["-o"]) == 5));
				return 0;
			});
	}
	{
		lyra::main()("-o", -1.0f)(
			args { "main" },
			[&](lyra::main & m)
			{
				test(REQUIRE(int(m["-o"]) == -1));
				return 0;
			});
	}
	{
		lyra::main()("-o", std::vector<int>())(
			args { "main" },
			[&](lyra::main & m)
			{
				std::vector<int> o = m["-o"];
				test
					(REQUIRE(o.size() == 0));
				return 0;
			});
	}
	{
		lyra::main()("-o", std::vector<int>())(
			args { "main", "-o0" },
			[&](lyra::main & m)
			{
				std::vector<int> o = m["-o"];
				test
					(REQUIRE(o.size() == 1))
					(REQUIRE(o[0] == 0));
				return 0;
			});
	}
	{
		lyra::main()("-o", std::vector<int>())(
			args { "main", "-o0", "-o1", "-o2" },
			[&](lyra::main & m)
			{
				std::vector<int> o = m["-o"];
				test
					(REQUIRE(o.size() == 3))
					(REQUIRE(o[0] == 0))
					(REQUIRE(o[1] == 1))
					(REQUIRE(o[2] == 2));
				return 0;
			});
	}
	{
		lyra::main()("-o", "vvv")(
			args { "main", "-o", "thing" },
			[&](lyra::main & m)
			{
				test(REQUIRE(std::string(m["-o"]) == "thing"));
				return 0;
			});
	}

	return test;
}

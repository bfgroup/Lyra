/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

#include <string>

int main()
{
	bfg::mini_test::scope test;

	{
		bool verbose = false;
		bool dryrun = false;
		std::string name;
		auto cli = lyra::cli();
		cli.add_argument(lyra::opt(verbose)["-v"]["--verbose"].optional());
		cli.add_argument(lyra::opt(dryrun)["-d"]["--dry-run"].optional());
		cli.add_argument(lyra::opt(name, "NAME")["-n"]["--name"].optional());
		{
			verbose = false;
			dryrun = false;
			name = "";
			auto result = cli.parse({ "test_app", "-v" });
			test(REQUIRE(result));
			test(REQUIRE(verbose));
			test(REQUIRE(!dryrun));
			test(REQUIRE(name.empty()));
		}
		{
			verbose = false;
			dryrun = false;
			name = "";
			auto result = cli.parse({ "test_app", "-vd" });
			test(REQUIRE(result));
			test(REQUIRE(verbose));
			test(REQUIRE(dryrun));
			test(REQUIRE(name.empty()));
		}
		{
			verbose = false;
			dryrun = false;
			name = "";
			auto result = cli.parse({ "test_app", "-vd", "-n", "Lyra" });
			test(REQUIRE(result));
			test(REQUIRE(verbose));
			test(REQUIRE(dryrun));
			test(REQUIRE(name == "Lyra"));
		}
		{
			verbose = false;
			dryrun = false;
			name = "";
			auto result = cli.parse({ "test_app", "-vdn", "Lyra" });
			test(REQUIRE(result));
			test(REQUIRE(verbose));
			test(REQUIRE(dryrun));
			test(REQUIRE(name == "Lyra"));
		}
	}
	return test;
}

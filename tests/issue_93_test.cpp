/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/lyra.hpp>

#include <memory>
#include <string>

int main()
{
	auto cli = lyra::cli {};

	auto f = [p = std::make_unique<int>(1)](bool) {};
	cli.add_argument(lyra::opt(f));

	auto g = [p = std::make_unique<int>(1)](std::string) {};
	cli.add_argument(lyra::opt(g, "hint"));
	cli.add_argument(lyra::arg(g, "hint"));
}

/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "base_lib.hpp"
#include <lyra/lyra.hpp>

namespace L {
int lib_args(int argc, const char ** argv)
{
	using namespace lyra;
	bool flag = false;
	auto p = cli() | opt(flag, "true|false")["-f"]("A flag");

	flag = false;
	auto result = p.parse(args(argc, argv));
	return bool(result);
}
} // namespace L

/*
Copyright 2019-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/lyra.hpp>
#include "base_lib.hpp"

int main(int argc, const char ** argv)
{
	using namespace lyra;
	bool flag = false;
	auto p = cli() | opt( flag, "true|false" )
			["-f"]
			("A flag");

	flag = false;
	auto result = p.parse(args(argc, argv));
	return bool(result) && (L::lib_args(argc, argv) > 0);
}

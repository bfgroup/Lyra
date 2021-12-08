/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/detail/trait_utils.hpp>
#include "mini_test.hpp"
#include <string>

template <typename L>
constexpr bool is_invocable_v(
	L,
	typename std::enable_if<lyra::detail::is_invocable<L>::value, int>::type = 0)
{
	return true;
}

template <typename T>
constexpr bool is_invocable_v(
	T,
	typename std::enable_if<!lyra::detail::is_invocable<T>::value, char>::type = 0)
{
	return false;
}

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	{
		test
			(REQUIRE(
				(!lyra::detail::is_callable<int, int>::value)
			))
			(REQUIRE(
				(!lyra::detail::is_callable<float, int, int>::value)
			))
			(REQUIRE(
				(!lyra::detail::is_callable<std::string, int>::value)
			))
			;
	}
	{
		auto f0 = []() -> bool { return false; };
		auto f1 = [](int x) -> bool { return x > 1; };
		auto f2 = [](int x, float y) -> bool { return x > y; };
		test
			(REQUIRE(
				(lyra::detail::is_callable<decltype(f0)>::value)
			))
			(REQUIRE(
				(lyra::detail::is_callable<decltype(f1), int>::value)
			))
			(REQUIRE(
				(lyra::detail::is_callable<decltype(f2), int, int>::value)
			))
			;
	}

	{
		test
			(REQUIRE(
				(!lyra::detail::is_invocable<int>::value)
			))
			(REQUIRE(
				(!lyra::detail::is_invocable<float>::value)
			))
			(REQUIRE(
				(!lyra::detail::is_invocable<std::string>::value)
			))
			;
	}
	{
		auto f0 = []() -> bool { return false; };
		auto f1 = [](int x) -> bool { return x > 1; };
		auto f2 = [](int x, float y) -> bool { return x > y; };
		test
			(REQUIRE(
				(lyra::detail::is_invocable<decltype(f0)>::value)
			))
			(REQUIRE(
				(lyra::detail::is_invocable<decltype(f1)>::value)
			))
			(REQUIRE(
				(lyra::detail::is_invocable<decltype(f2)>::value)
			))
			;
	}
	{
		test
			(REQUIRE(
				is_invocable_v([]() -> bool { return false; })
			))
			(REQUIRE(
				is_invocable_v([](int x) -> bool { return x > 1; })
			))
			(REQUIRE(
				is_invocable_v([](int x, float y) -> bool { return x > y; })
			))
			;
	}

	return test;
}

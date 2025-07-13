/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include "mini_test.hpp"
#include <lyra/detail/trait_utils.hpp>
#include <lyra/detail/unary_lambda_traits.hpp>
#include <memory>
#include <string>
#include <vector>

template <typename L>
constexpr bool is_invocable_v(L,
	typename std::enable_if<lyra::detail::is_invocable<L>::value, int>::type
	= 0)
{
	return true;
}

template <typename T>
constexpr bool is_invocable_v(T,
	typename std::enable_if<!lyra::detail::is_invocable<T>::value, char>::type
	= 0)
{
	return false;
}

int main()
{
	using namespace lyra;
	bfg::mini_test::scope test;

	{
		test(REQUIRE((!lyra::detail::is_callable<int, int>::value)));
		test(REQUIRE((!lyra::detail::is_callable<float, int, int>::value)));
		test(REQUIRE((!lyra::detail::is_callable<std::string, int>::value)));
	}
	{
		auto f0 = []() -> bool { return false; };
		auto f1 = [](int x) -> bool { return x > 1; };
		auto f2 = [](int x, float y) -> bool { return x > y; };
		test(REQUIRE((lyra::detail::is_callable<decltype(f0)>::value)));
		test(REQUIRE((lyra::detail::is_callable<decltype(f1), int>::value)));
		test(REQUIRE(
			(lyra::detail::is_callable<decltype(f2), int, int>::value)));
	}

	{
		test(REQUIRE((!lyra::detail::is_invocable<int>::value)));
		test(REQUIRE((!lyra::detail::is_invocable<float>::value)));
		test(REQUIRE((!lyra::detail::is_invocable<std::string>::value)));
	}
	{
		auto f0 = []() -> bool { return false; };
		auto f1 = [](int x) -> bool { return x > 1; };
		auto f2 = [](int x, float y) -> bool { return x > y; };
		test(REQUIRE((lyra::detail::is_invocable<decltype(f0)>::value)));
		test(REQUIRE((lyra::detail::is_invocable<decltype(f1)>::value)));
		test(REQUIRE((lyra::detail::is_invocable<decltype(f2)>::value)));
	}
	{
		test(REQUIRE(is_invocable_v([]() -> bool { return false; })));
		test(REQUIRE(is_invocable_v([](int x) -> bool { return x > 1; })));
		test(REQUIRE(
			is_invocable_v([](int x, float y) -> bool { return x > y; })));
	}
#if (__cplusplus > 201103L)
	{
		auto f0 = [p = std::make_unique<int>(1)](std::string) {};
		test(REQUIRE((lyra::detail::is_invocable<decltype(f0)>::value)));
		test(REQUIRE(
			(lyra::detail::is_invocable<decltype(std::move(f0))>::value)));
	}
#endif
	{
		struct F0
		{
			std::unique_ptr<int> i;
			bool operator()() const { return false; }
		};
		F0 f0;
		test(REQUIRE((lyra::detail::is_invocable<decltype(f0)>::value)));
		test(REQUIRE(
			(lyra::detail::is_invocable<decltype(std::move(f0))>::value)));
	}
	{
		auto f0 = []() -> bool { return false; };
		auto f1 = [](int x) -> bool { return x > 1; };
		struct F2
		{
			std::unique_ptr<int> i;
			bool operator()(int) const { return false; }
		};
		F2 f2;
		test(
			REQUIRE(!lyra::detail::unary_lambda_traits<decltype(f0)>::isValid));
		test(REQUIRE(lyra::detail::unary_lambda_traits<decltype(f1)>::isValid));
		test(REQUIRE(lyra::detail::unary_lambda_traits<decltype(f2)>::isValid));
		test(REQUIRE(
			lyra::detail::unary_lambda_traits<lyra::detail::remove_cvref<
				decltype(std::move(f2))>::type>::isValid));
	}
	{
		std::string s;
		std::vector<int> v;
		test(REQUIRE(!lyra::detail::is_invocable<decltype(s)>::value));
		test(REQUIRE(!lyra::detail::is_invocable<decltype(v)>::value));
	}
	{
		test(REQUIRE(lyra::detail::is_character<char>::value));
		test(REQUIRE(lyra::detail::is_character<wchar_t>::value));
		test(REQUIRE(lyra::detail::is_character<unsigned char>::value));
		test(REQUIRE(lyra::detail::is_character<const char>::value));
		test(REQUIRE(lyra::detail::is_character<const char &>::value));
	}

	return test;
}

// Copyright René Ferdinand Rivera Morell
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_DETAIL_TRAIT_UTILS_HPP
#define LYRA_DETAIL_TRAIT_UTILS_HPP

#include <type_traits>
// #include <utility>

namespace lyra { namespace detail {

template <class T>
struct remove_cvref
{
	using type =
		typename std::remove_cv<typename std::remove_reference<T>::type>::type;
};

// Checks that F can be called, with an unspecified set of arguments.
//
// Currently this only detects function objects, like lambdas.
// Where the operator() is not templated.
template <class F>
struct is_invocable
{
	template <class U>
	static auto test(U * p)
		-> decltype((&U::operator()), void(), std::true_type());

	template <class U>
	static auto test(...) -> decltype(std::false_type());

	static constexpr bool value
		= decltype(test<typename remove_cvref<F>::type>(nullptr))::value;
};

// Borrowed from https://wg21.link/p2098r1
template <class T, template <class...> class Primary>
struct is_specialization_of : std::false_type
{};
template <template <class...> class Primary, class... Args>
struct is_specialization_of<Primary<Args...>, Primary> : std::true_type
{};

// Composite trait for just character types.
template <typename C>
struct is_character
{
	using bare_t = typename remove_cvref<C>::type;
	static constexpr bool value = false || std::is_same<char, bare_t>::value
		|| std::is_same<signed char, bare_t>::value
		|| std::is_same<unsigned char, bare_t>::value
		|| std::is_same<wchar_t, bare_t>::value
#if (__cplusplus >= 202002L)
		|| std::is_same<char8_t, bare_t>::value
#endif
		|| std::is_same<char16_t, bare_t>::value
		|| std::is_same<char32_t, bare_t>::value;
};

}} // namespace lyra::detail

#endif

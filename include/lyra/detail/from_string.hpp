// Copyright René Ferdinand Rivera Morell
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_DETAIL_FROM_STRING_HPP
#define LYRA_DETAIL_FROM_STRING_HPP

#include "lyra/detail/trait_utils.hpp"

#include <cctype>
#include <sstream>
#include <string>
#include <type_traits>

#if defined(__has_include)
#	if __has_include(<version>)
#		include <version>
#	elif __has_include(<ciso646>)
#		include <ciso646>
#	endif

#	ifndef LYRA_CONFIG_OPTIONAL_TYPE
#		if __has_include(<optional>) \
		&& defined(__cpp_lib_optional) && (__cpp_lib_optional >= 201606L)
#			include <optional>
#			define LYRA_CONFIG_OPTIONAL_TYPE std::optional
#		endif
#	endif

#	if __has_include(<filesystem>) \
	&& defined(__cpp_lib_filesystem) && (__cpp_lib_filesystem >= 201703L)
#		include <filesystem>
#		define LYRA_HAS_FILESYSTEM_PATH
#	endif

#	if __has_include(<charconv>) \
	&& defined(__cpp_lib_to_chars) && (__cpp_lib_to_chars >= 201611L)
#		include <array>
#		include <charconv>
#		include <system_error>
#		define LYRA_HAS_CHARCONV
#	endif
#endif

namespace lyra { namespace detail {

template <typename T, typename = void>
struct is_charconv_convertible : std::false_type
{ };

#ifdef LYRA_HAS_CHARCONV
template <typename T>
struct is_charconv_convertible<T,
	typename std::enable_if<std::is_integral<T>::value
		&& !std::is_same<T, bool>::value>::type> : std::true_type
{ };

template <typename T>
struct is_charconv_convertible<T,
	typename std::enable_if<std::is_floating_point<T>::value>::type>
	: std::true_type
{ };
#endif

template <typename T,
	typename std::enable_if<!is_charconv_convertible<T>::value, int>::type = 0>
inline bool to_string(const T & source, std::string & target)
{
	std::stringstream ss;
	ss << source;
	ss >> target;
	return !ss.fail();
}

#ifdef LYRA_HAS_CHARCONV
template <typename T,
	typename std::enable_if<is_charconv_convertible<T>::value, int>::type = 0>
inline bool to_string(const T & source, std::string & target)
{
	std::array<char, 24> buffer { };

	auto result
		= std::to_chars(buffer.data(), buffer.data() + buffer.size(), source);
	if (result.ec != std::errc())
	{
		return false;
	}

	target = std::string(buffer.data(), result.ptr);
	return true;
}
#endif

inline bool to_string(const std::string & source, std::string & target)
{
	target = source;
	return true;
}

inline bool to_string(const char * source, std::string & target)
{
	target = source;
	return true;
}

inline bool to_string(bool source, std::string & target)
{
	target = source ? "true" : "false";
	return true;
}

#ifdef LYRA_HAS_FILESYSTEM_PATH
inline bool to_string(
	const std::filesystem::path & source, std::string & target)
{
	target = source.string();
	return true;
}
#endif

#ifdef LYRA_CONFIG_OPTIONAL_TYPE
template <typename T>
inline bool to_string(
	LYRA_CONFIG_OPTIONAL_TYPE<T> & source, std::string & target)
{
	if (source)
		return to_string(*source, target);
	else
		target = "<nullopt>";
	return true;
}
#endif // LYRA_CONFIG_OPTIONAL_TYPE

template <typename, typename = void>
struct is_convertible_from_string : std::false_type
{ };

template <typename T>
struct is_convertible_from_string<T,
	typename std::enable_if<std::is_arithmetic<T>::value>::type>
	: std::true_type
{ };

// Validates format of given value strings before conversion. This default
// template return true always.
template <typename, typename = void>
struct validate_from_string
{
	static bool validate(const std::string &) { return true; }
};

// Validates that a to be parsed unsigned integer only contains number
// digits.
template <typename T>
struct validate_from_string<T,
	typename std::enable_if<
		std::is_unsigned<typename detail::remove_cvref<T>::type>::value>::type>
{
	static bool validate(const std::string & s)
	{
		return s.find_first_not_of("0123456789") == std::string::npos;
	}
};

// Validates that a to be parsed signed integer only contains a sign and
// number digits.
template <typename T>
struct validate_from_string<T,
	typename std::enable_if<
		std::is_integral<typename detail::remove_cvref<T>::type>::value
		&& std::is_signed<typename detail::remove_cvref<T>::type>::value>::type>
{
	static bool validate(const std::string & s)
	{
		return s.find_first_not_of("-0123456789") == std::string::npos;
	}
};

template <typename S,
	typename T,
	typename std::enable_if<!is_charconv_convertible<T>::value, int>::type = 0>
inline bool from_string(S const & source, T & target)
{
	std::stringstream ss;
	// Feed what we want to convert into the stream so that we can convert it
	// on extraction to the target type.
	ss << source;
	// Check that the source string data is valid. This check depends on the
	// target type.
	if (!validate_from_string<T>::validate(ss.str())) return false;
	T temp { };
	ss >> temp;
	if (!ss.fail() && ss.eof())
	{
		target = temp;
		return true;
	}
	return false;
}

#ifdef LYRA_HAS_CHARCONV
template <typename S,
	typename T,
	typename std::enable_if<is_charconv_convertible<T>::value, int>::type = 0>
inline bool from_string(S const & source, T & target)
{
	T tmp { };
	std::from_chars_result result
		= std::from_chars(source.data(), source.data() + source.size(), tmp);

	if (result.ec == std::errc()
		&& result.ptr == (source.data() + source.size()))
	{
		target = tmp;
		return true;
	}

	return false;
}
#endif

template <typename S, typename... C>
inline bool from_string(S const & source, std::basic_string<C...> & target)
{
	to_string(source, target);
	return true;
}

template <typename T>
struct is_convertible_from_string<T,
	typename std::enable_if<std::is_same<T, bool>::value>::type>
	: std::true_type
{ };

template <typename S>
inline bool from_string(S const & source, bool & target)
{
	std::string srcLC;
	to_string(source, srcLC);
	for (std::string::value_type & c : srcLC)
		c = static_cast<std::string::value_type>(std::tolower(c));
	if (srcLC == "y" || srcLC == "1" || srcLC == "true" || srcLC == "yes"
		|| srcLC == "on")
		target = true;
	else if (srcLC == "n" || srcLC == "0" || srcLC == "false" || srcLC == "no"
		|| srcLC == "off")
		target = false;
	else
		return false;
	return true;
}

#ifdef LYRA_HAS_FILESYSTEM_PATH
template <typename S>
inline bool from_string(S const & source, std::filesystem::path & target)
{
	target = { source };
	return true;
}
#endif

#ifdef LYRA_CONFIG_OPTIONAL_TYPE
template <typename T>
struct is_convertible_from_string<T,
	typename std::enable_if<
		is_specialization_of<T, LYRA_CONFIG_OPTIONAL_TYPE>::value>::type>
	: std::true_type
{ };

template <typename S, typename T>
inline bool from_string(S const & source, LYRA_CONFIG_OPTIONAL_TYPE<T> & target)
{
	std::string srcLC;
	to_string(source, srcLC);
	for (std::string::value_type & c : srcLC)
		c = static_cast<std::string::value_type>(::tolower(c));
	if (srcLC == "<nullopt>")
	{
		target.reset();
		return true;
	}
	else
	{
		T temp;
		auto str_result = from_string(source, temp);
		if (str_result) target = std::move(temp);
		return str_result;
	}
}
#endif // LYRA_CONFIG_OPTIONAL_TYPE

}} // namespace lyra::detail

#endif

/*
Copyright René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef BFG_MINI_TEST_HPP
#define BFG_MINI_TEST_HPP

#include <iostream>
#include <string>

namespace bfg { namespace mini_test {

struct scope
{
	unsigned pass_count = 0;
	unsigned fail_count = 0;
	operator int() const { return static_cast<int>(fail_count); }
	scope & operator()(
		bool pass, const std::string & expression, char const * file, int line)
	{
		return (*this)("", pass, expression, file, line);
	}
	scope & operator()(const std::string & message,
		bool pass,
		const std::string & expression,
		char const * file,
		int line)
	{
		if (pass)
			pass_count += 1;
		else
		{
			fail_count += 1;
			if (!message.empty()) std::cerr << "[ " << message << " ] ";
			std::cerr << file << "(" << line << "): test '" << expression
					  << "' failed\n";
		}
		return *this;
	}
	template <typename R>
	void check_result(const R & result) const
	{
		auto m = result.message();
		if (!result)
			std::cerr << "[ERROR] " << m << "\n";
		else
			std::cout << "[DEBUG] " << m << "\n";
	}
};

#define CONTEXT __FILE__, __LINE__
#define REQUIRE(condition) (bool(condition)), #condition, __FILE__, __LINE__
#define COND_HAS_STRING(message_a, string_a) \
	((message_a).find(string_a) != std::string::npos)

}} // namespace bfg::mini_test

#endif

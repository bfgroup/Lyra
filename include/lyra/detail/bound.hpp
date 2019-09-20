// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_DETAIL_BOUND_HPP
#define LYRA_DETAIL_BOUND_HPP

#include "lyra/detail/invoke_lambda.hpp"
#include "lyra/detail/parse.hpp"
#include "lyra/detail/unary_lambda_traits.hpp"
#include <string>

namespace lyra
{
namespace detail
{
	struct NonCopyable
	{
		NonCopyable() = default;
		NonCopyable(NonCopyable const&) = delete;
		NonCopyable(NonCopyable&&) = delete;
		NonCopyable& operator=(NonCopyable const&) = delete;
		NonCopyable& operator=(NonCopyable&&) = delete;
	};

	struct BoundRef : NonCopyable
	{
		virtual ~BoundRef() = default;
		virtual auto isContainer() const -> bool { return false; }
		virtual auto isFlag() const -> bool { return false; }
	};
	struct BoundValueRefBase : BoundRef
	{
		virtual auto setValue(std::string const& arg) -> parser_result = 0;
	};
	struct BoundFlagRefBase : BoundRef
	{
		virtual auto setFlag(bool flag) -> parser_result = 0;
		virtual auto isFlag() const -> bool { return true; }
	};

	template <typename T>
	struct BoundValueRef : BoundValueRefBase
	{
		T& m_ref;

		explicit BoundValueRef(T& ref)
			: m_ref(ref)
		{
		}

		auto setValue(std::string const& arg) -> parser_result override
		{
			return parse_string(arg, m_ref);
		}
	};

	template <typename T>
	struct BoundValueRef<std::vector<T>> : BoundValueRefBase
	{
		std::vector<T>& m_ref;

		explicit BoundValueRef(std::vector<T>& ref)
			: m_ref(ref)
		{
		}

		auto isContainer() const -> bool override { return true; }

		auto setValue(std::string const& arg) -> parser_result override
		{
			T temp;
			auto result = parse_string(arg, temp);
			if (result) m_ref.push_back(temp);
			return result;
		}
	};

	struct BoundFlagRef : BoundFlagRefBase
	{
		bool& m_ref;

		explicit BoundFlagRef(bool& ref)
			: m_ref(ref)
		{
		}

		auto setFlag(bool flag) -> parser_result override
		{
			m_ref = flag;
			return parser_result::ok(parser_result_type::matched);
		}
	};

	template <typename L>
	struct BoundLambda : BoundValueRefBase
	{
		L m_lambda;

		static_assert(
			unary_lambda_traits<L>::isValid,
			"Supplied lambda must take exactly one argument");
		explicit BoundLambda(L const& lambda)
			: m_lambda(lambda)
		{
		}

		auto setValue(std::string const& arg) -> parser_result override
		{
			return invokeLambda<typename unary_lambda_traits<L>::ArgType>(
				m_lambda, arg);
		}
	};

	template <typename L>
	struct BoundFlagLambda : BoundFlagRefBase
	{
		L m_lambda;

		static_assert(
			unary_lambda_traits<L>::isValid,
			"Supplied lambda must take exactly one argument");
		static_assert(
			std::is_same<typename unary_lambda_traits<L>::ArgType, bool>::value,
			"flags must be boolean");

		explicit BoundFlagLambda(L const& lambda)
			: m_lambda(lambda)
		{
		}

		auto setFlag(bool flag) -> parser_result override
		{
			return LambdaInvoker<typename unary_lambda_traits<L>::ReturnType>::
				invoke(m_lambda, flag);
		}
	};
} // namespace detail
} // namespace lyra

#endif

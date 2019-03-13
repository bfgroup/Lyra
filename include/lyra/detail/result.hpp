// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_DETAIL_RESULT_HPP
#define LYRA_DETAIL_RESULT_HPP

#include <cassert>
#include <string>

namespace lyra
{
namespace detail
{
    class result_base
    {
        public:
        enum Type
        {
            Ok,
            LogicError,
            RuntimeError
        };

        protected:
        result_base(Type type)
            : m_type(type)
        {
        }

        virtual ~result_base() = default;

        Type m_type;
    };

    template <typename T>
    class result_value_base : public result_base
    {
        public:
        auto value() const -> T const&
        {
            return m_value;
        }

        protected:
        result_value_base(Type type)
            : result_base(type)
        {
        }

        result_value_base(result_value_base const& other)
            : result_base(other)
        {
            if (this->m_type == result_base::Ok)
                new (&m_value) T(other.m_value);
        }

        result_value_base(Type, T const& value)
            : result_base(Ok)
        {
            new (&m_value) T(value);
        }

        auto operator=(result_value_base const& other) -> result_value_base&
        {
            if (this->m_type == result_base::Ok)
                m_value.~T();
            result_base::operator=(other);
            if (this->m_type == result_base::Ok)
                new (&m_value) T(other.m_value);
            return *this;
        }

        ~result_value_base() override
        {
            if (this->m_type == Ok)
                m_value.~T();
        }

        union
        {
            T m_value;
        };
    };

    template <>
    class result_value_base<void> : public result_base
    {
        protected:
        using result_base::result_base;
    };

    template <typename T = void>
    class basic_result : public result_value_base<T>
    {
        public:
        template <typename U>
        explicit basic_result(basic_result<U> const& other)
            : result_value_base<T>(other.type())
            , m_errorMessage(other.errorMessage())
        {
            assert(type() != result_base::Ok);
        }

        template <typename U>
        static auto ok(U const& value) -> basic_result
        {
            return { result_base::Ok, value };
        }
        static auto ok() -> basic_result { return { result_base::Ok }; }
        static auto logicError(std::string const& message) -> basic_result
        {
            return { result_base::LogicError, message };
        }
        static auto runtimeError(std::string const& message) -> basic_result
        {
            return { result_base::RuntimeError, message };
        }

        explicit operator bool() const { return this->m_type == result_base::Ok; }
        auto type() const -> result_base::Type { return this->m_type; }
        auto errorMessage() const -> std::string { return m_errorMessage; }

        protected:
        std::string m_errorMessage; // Only populated if resultType is an error

        basic_result(result_base::Type type, std::string const& message)
            : result_value_base<T>(type)
            , m_errorMessage(message)
        {
            assert(this->m_type != result_base::Ok);
        }

        using result_value_base<T>::result_value_base;
    };
} // namespace detail
} // namespace lyra

#endif

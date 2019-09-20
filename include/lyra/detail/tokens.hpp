// Copyright 2018-2019 Rene Rivera
// Copyright 2017 Two Blue Cubes Ltd. All rights reserved.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_DETAIL_TOKENS_HPP
#define LYRA_DETAIL_TOKENS_HPP

#include <string>
#include <vector>

namespace lyra
{
namespace detail
{
	// Wraps a token coming from a token stream. These may not directly
	// correspond to strings as a single string may encode an option + its
	// argument if the : or = form is used
	enum class token_type
	{
		unknown,
		option,
		argument
	};
	struct token
	{
		token_type type;
		std::string name;

		token()
			: type(token_type::unknown)
		{
		}
		token(const token& other) = default;
		token(token_type t, const std::string& n)
			: type(t)
			, name(n)
		{
		}
	};

	// Abstracts iterators into args with option arguments uniformly handled
	class token_iterator
	{
		public:
		template <typename Span>
		explicit token_iterator(
			Span const& args, std::string const& dels,
			std::string const& opt_prefix)
			: token_iterator(args.begin(), args.end(), dels, opt_prefix)
		{
		}

		token_iterator(
			std::vector<std::string>::const_iterator it,
			std::vector<std::string>::const_iterator itEnd,
			std::string const& dels, std::string const& opt_prefix)
			: it(it)
			, itEnd(itEnd)
			, delimiters(dels)
			, optionPrefix(opt_prefix)
		{
			loadBuffer();
		}

		explicit operator bool() const
		{
			return !m_tokenBuffer.empty() || it != itEnd;
		}

		auto count() const -> size_t
		{
			return m_tokenBuffer.size() + (itEnd - it);
		}

		// UB if boot(*this) == false.
		auto operator*() const -> token { return *(operator->()); }

		auto operator-> () const -> token const*
		{
			return !m_tokenBuffer.empty() ? &m_tokenBuffer.front() : nullptr;
		}

		auto operator++() -> token_iterator&
		{
			if (m_tokenBuffer.size() >= 2)
			{
				m_tokenBuffer.erase(m_tokenBuffer.begin());
			}
			else
			{
				if (it != itEnd) ++it;
				loadBuffer();
			}
			return *this;
		}

		private:
		std::vector<std::string>::const_iterator it;
		std::vector<std::string>::const_iterator itEnd;
		std::vector<token> m_tokenBuffer;
		std::string delimiters;
		std::string optionPrefix;

		void loadBuffer()
		{
			m_tokenBuffer.resize(0);

			// Skip any empty strings
			while (it != itEnd && it->empty()) ++it;

			if (it != itEnd)
			{
				auto const& next = *it;
				if (isOptPrefix(next[0]))
				{
					auto delimiterPos = next.find_first_of(delimiters);
					if (delimiterPos != std::string::npos)
					{
						m_tokenBuffer.emplace_back(
							token_type::option, next.substr(0, delimiterPos));
						m_tokenBuffer.emplace_back(
							token_type::argument,
							next.substr(delimiterPos + 1));
					}
					else
					{
						if (!isOptPrefix(next[1]) && next.size() > 2)
						{
							std::string opt;
							opt += optionPrefix[0];
							opt += " ";
							for (size_t i = 1; i < next.size(); ++i)
							{
								opt[1] = next[i];
								m_tokenBuffer.emplace_back(
									token_type::option, opt);
							}
						}
						else
						{
							m_tokenBuffer.emplace_back(
								token_type::option, next);
						}
					}
				}
				else
				{
					m_tokenBuffer.emplace_back(token_type::argument, next);
				}
			}
		}

		inline auto isOptPrefix(char c) -> bool
		{
			auto r = optionPrefix.find(c) != std::string::npos;
			return r;
		}
	};
} // namespace detail
} // namespace lyra

#endif

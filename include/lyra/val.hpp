// Copyright 2020 René Ferdinand Rivera Morell
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LYRA_VAL_HPP
#define LYRA_VAL_HPP

#include "lyra/detail/bound.hpp"
#include <memory>

namespace lyra {

/* tag::reference[]

[#lyra_val]
= `lyra::val`

*/ // end::reference[]
template <typename T>
detail::BoundVal<T> val(T && v)
{
	return detail::BoundVal<T>(std::forward<T>(v));
}

inline detail::BoundVal<std::string> val(const char * v)
{
	return detail::BoundVal<std::string>(v);
}

} // namespace lyra

#endif

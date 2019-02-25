/*
Copyright Rene Rivera 2019
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#include <lyra/lyra.hpp>

int main()
{
    using namespace lyra;
    std::string name;
    auto p = Opt(name, "name")["-n"]["--name"]("the name to use");
    p.parse( Args{ "TestApp", "-f" } );
    return name == "" ? 0 : 1;
}

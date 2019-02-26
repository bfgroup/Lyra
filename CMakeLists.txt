# Copyright Rene Rivera 2019
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt

# NOTE:
#   CMake support for Lyra is currently experimental at best and the
#   interface is likely to change in the future
#
# This file provides minimal cmake support (no unit-tests,
# no installation) for integration into a "host" cmake project
# via the "add_subdirectory( <path-to-lyra> )" command.
#
# Other cmake targets can then use the public target name
# "BFG::Lyra" in order to express their dependency
# on this library. I.e:
#
# target_link_libraries( <my-exe/lib> PUBLIC BFG::Lyra )

# Only need the basic minimum of project, add_library, and
# target_include_directories commands.
cmake_minimum_required( VERSION 3.0 )

# Don't set VERSION, as that's a pita to keep up to date with the version
# header. And don't set LANGUAGES as we are multi-language and header
# only, so it's irrelevant.
project( BFG_Lyra )

# Simple INTERFACE, and header only, library target.
add_library( bfg_lyra INTERFACE )

# The only usage requirement is include dir for consumers.
target_include_directories( bfg_lyra INTERFACE include )

# Add an alias to public name.
add_library( BFG::Lyra ALIAS bfg_lyra )

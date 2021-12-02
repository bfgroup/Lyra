# Copyright 2019 René Ferdinand Rivera Morell
# Copyright 2020 Jayesh Vinay Badwaik
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
# "bfg::lyra" in order to express their dependency
# on this library. I.e:
#
# target_link_libraries( <my-exe/lib> PUBLIC bfg::lyra )

# Only need the basic minimum of project, add_library, and
# target_include_directories commands.
cmake_minimum_required( VERSION 3.0 )

# Don't set VERSION, as that's a pita to keep up to date with the version
# header. And don't set LANGUAGES as we are multi-language and header
# only, so it's irrelevant.
project( lyra )

# Simple INTERFACE, and header only, library target.
add_library( lyra INTERFACE )

# The only usage requirement is include dir for consumers.
target_include_directories(
  lyra
  INTERFACE
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  )

# Add an alias to public name.
add_library( bfg::lyra ALIAS lyra )

## Installation Code
include(GNUInstallDirs)

include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/data/cmake/lyraConfig.cmake.in
  ${PROJECT_BINARY_DIR}/lyraConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/lyra
  )

install(
  TARGETS lyra
  EXPORT lyraTarget
  )

install(
  EXPORT lyraTarget
  FILE  lyraTarget.cmake
  NAMESPACE bfg::
  DESTINATION ${CMAKE_INSTALL_DATADIR}/lyra/cmake
  )

install(
  DIRECTORY ${PROJECT_SOURCE_DIR}/include
  DESTINATION  ${CMAKE_INSTALL_PREFIX}
  )

install(
  FILES
  ${PROJECT_BINARY_DIR}/lyraConfig.cmake
  DESTINATION ${CMAKE_INSTALL_DATADIR}/lyra/cmake/
  )




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
cmake_minimum_required( VERSION 3.12 )

# Don't set VERSION, as that's a pita to keep up to date with the version
# header. And don't set LANGUAGES as we are multi-language and header
# only, so it's irrelevant.
project( lyra
	VERSION "1.6"
	DESCRIPTION "A simple to use, composing, header only, command line arguments parser for C++ 11 and beyond."
	HOMEPAGE_URL "https://github.com/bfgroup/Lyra"
)
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "${PROJECT_NAME} developers")
set(CPACK_PACKAGE_DESCRIPTION "${PROJECT_DESCRIPTION}")
set(CPACK_DEBIAN_PACKAGE_NAME "lib${CPACK_PACKAGE_NAME}-dev")
set(CPACK_RPM_PACKAGE_NAME "lib${CPACK_PACKAGE_NAME}-devel")
set(CPACK_PACKAGE_HOMEPAGE_URL "${PROJECT_HOMEPAGE_URL}")
set(CPACK_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.adoc")

include(GNUInstallDirs)
string(REPLACE "/${CMAKE_LIBRARY_ARCHITECTURE}" "" INSTALL_LIBDIR_ARCHIND "${CMAKE_INSTALL_LIBDIR}")

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

set(PkgConfigTemplatesDir "${CMAKE_CURRENT_SOURCE_DIR}/data/pkgconfig")

## Installation Code
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/data/cmake/lyraConfig.cmake.in
  ${PROJECT_BINARY_DIR}/lyraConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/lyra/cmake
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
  DESTINATION ${CMAKE_INSTALL_DATADIR}/lyra/cmake
  )

set(PKG_CONFIG_FILE_NAME "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc")
configure_file("${PkgConfigTemplatesDir}/pkgconfig.pc.in" "${PKG_CONFIG_FILE_NAME}" @ONLY)
install(FILES "${PKG_CONFIG_FILE_NAME}"
	DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

include(CPack)



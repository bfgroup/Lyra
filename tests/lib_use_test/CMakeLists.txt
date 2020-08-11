# Copyright 2020 Jayesh Vinay Badwaik
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt


cmake_minimum_required( VERSION 3.0 )
project(base_lib_test)

message(STATUS "${CMAKE_PREFIX_PATH}")

find_package(lyra REQUIRED)

include(CTest)
add_executable(base_exe base_exe.cpp)
target_sources(base_exe PRIVATE base_lib.cpp)
target_include_directories(base_exe PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(base_exe PUBLIC bfg::lyra)
add_test(NAME base_exe
         COMMAND base_exe -f)

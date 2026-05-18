# Use, modification, and distribution are
# subject to the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Copyright René Ferdinand Rivera Morell.

import argparse
import json


class CXXPipelines(object):

    matrix_compilers = {
        "gcc-16": {
            "NAME": "GCC 16",
            "EXE": "g++-16",
            "TOOLSET": "gcc-16",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/gcc-ubuntu:16",
            "GROUP": "Linux",
        },
        "gcc-15": {
            "NAME": "GCC 15",
            "EXE": "g++-15",
            "TOOLSET": "gcc-15",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/gcc-ubuntu:15",
            "GROUP": "Linux",
        },
        "gcc-14": {
            "NAME": "GCC 14",
            "EXE": "g++-14",
            "TOOLSET": "gcc-14",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/gcc-ubuntu:14",
            "GROUP": "Linux",
        },
        "gcc-13": {
            "NAME": "GCC 13",
            "EXE": "g++-13",
            "TOOLSET": "gcc-13",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/gcc-ubuntu:13",
            "GROUP": "Linux",
        },
        "gcc-12": {
            "NAME": "GCC 12",
            "EXE": "g++-12",
            "TOOLSET": "gcc-12",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/gcc-ubuntu:12",
            "GROUP": "Linux",
        },
        "gcc-11": {
            "NAME": "GCC 11",
            "EXE": "g++-11",
            "TOOLSET": "gcc-11",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/gcc-ubuntu:11",
            "GROUP": "Linux",
        },
        "gcc-10": {
            "NAME": "GCC 10",
            "EXE": "g++-10",
            "TOOLSET": "gcc-10",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/gcc-ubuntu:10",
            "GROUP": "Linux",
        },
        "gcc-9": {
            "NAME": "GCC 9",
            "EXE": "g++-9",
            "TOOLSET": "gcc-9",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/gcc-ubuntu:9",
            "GROUP": "Linux",
        },
        "gcc-8": {
            "NAME": "GCC 8",
            "EXE": "g++-8",
            "TOOLSET": "gcc-8",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/gcc-ubuntu:8",
            "GROUP": "Linux",
        },
        "gcc-7": {
            "NAME": "GCC 7",
            "EXE": "g++-7",
            "TOOLSET": "gcc-7",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/gcc-ubuntu:7",
            "GROUP": "Linux",
        },
        "gcc-6": {
            "NAME": "GCC 6",
            "EXE": "g++-6",
            "TOOLSET": "gcc-6",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/gcc-ubuntu:6",
            "GROUP": "Linux",
        },
        "gcc-5": {
            "NAME": "GCC 5",
            "EXE": "g++-5",
            "TOOLSET": "gcc-5",
            "CXXSTD": "11,14",
            "CONTAINER": "teeks99/gcc-ubuntu:5",
            "GROUP": "Linux",
        },
        "gcc-4.9": {
            "NAME": "GCC 4.9",
            "EXE": "g++-4.9",
            "TOOLSET": "gcc-4.9",
            "CXXSTD": "11",
            "CONTAINER": "teeks99/gcc-ubuntu:4.9",
            "GROUP": "Linux",
        },
        "gcc-4.8": {
            "NAME": "GCC 4.8",
            "EXE": "g++-4.8",
            "TOOLSET": "gcc-4.8",
            "CXXSTD": "11",
            "CONTAINER": "teeks99/gcc-ubuntu:4.8",
            "GROUP": "Linux",
        },
        "clang-23": {
            "NAME": "Clang 23",
            "EXE": "clang++-23",
            "TOOLSET": "clang-23",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:23",
            "GROUP": "Linux",
        },
        "clang-22": {
            "NAME": "Clang 22",
            "EXE": "clang++-22",
            "TOOLSET": "clang-22",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:22",
            "GROUP": "Linux",
        },
        "clang-21": {
            "NAME": "Clang 21",
            "EXE": "clang++-21",
            "TOOLSET": "clang-21",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:21",
            "GROUP": "Linux",
        },
        "clang-20": {
            "NAME": "Clang 20",
            "EXE": "clang++-20",
            "TOOLSET": "clang-20",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:20",
            "GROUP": "Linux",
        },
        "clang-19": {
            "NAME": "Clang 19",
            "EXE": "clang++-19",
            "TOOLSET": "clang-19",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:19",
            "GROUP": "Linux",
        },
        "clang-18": {
            "NAME": "Clang 18",
            "EXE": "clang++-18",
            "TOOLSET": "clang-18",
            "CXXSTD": "11,14,17,20,23",
            "CONTAINER": "teeks99/clang-ubuntu:18",
            "GROUP": "Linux",
        },
        "clang-17": {
            "NAME": "Clang 17",
            "EXE": "clang++-17",
            "TOOLSET": "clang-17",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/clang-ubuntu:17",
            "GROUP": "Linux",
        },
        "clang-16": {
            "NAME": "Clang 16",
            "EXE": "clang++-16",
            "TOOLSET": "clang-16",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/clang-ubuntu:16",
            "GROUP": "Linux",
        },
        "clang-15": {
            "NAME": "Clang 15",
            "EXE": "clang++-15",
            "TOOLSET": "clang-15",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/clang-ubuntu:15",
            "GROUP": "Linux",
        },
        "clang-14": {
            "NAME": "Clang 14",
            "EXE": "clang++-14",
            "TOOLSET": "clang-14",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/clang-ubuntu:14",
            "GROUP": "Linux",
        },
        "clang-13": {
            "NAME": "Clang 13",
            "EXE": "clang++-13",
            "TOOLSET": "clang-13",
            "CXXSTD": "11,14,17,20",
            "CONTAINER": "teeks99/clang-ubuntu:13",
            "GROUP": "Linux",
        },
        "clang-12": {
            "NAME": "Clang 12",
            "EXE": "clang++-12",
            "TOOLSET": "clang-12",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:12",
            "GROUP": "Linux",
        },
        "clang-11": {
            "NAME": "Clang 11",
            "EXE": "clang++-11",
            "TOOLSET": "clang-11",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:11",
            "GROUP": "Linux",
        },
        "clang-10": {
            "NAME": "Clang 10",
            "EXE": "clang++-10",
            "TOOLSET": "clang-10",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:10",
            "GROUP": "Linux",
        },
        "clang-9": {
            "NAME": "Clang 9",
            "EXE": "clang++-9",
            "TOOLSET": "clang-9",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:9",
            "GROUP": "Linux",
        },
        "clang-8": {
            "NAME": "Clang 8",
            "EXE": "clang++-8",
            "TOOLSET": "clang-8",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:8",
            "GROUP": "Linux",
        },
        "clang-7": {
            "NAME": "Clang 7",
            "EXE": "clang++-7",
            "TOOLSET": "clang-7",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:7",
            "GROUP": "Linux",
        },
        "clang-6": {
            "NAME": "Clang 6",
            "EXE": "clang++-6.0",
            "TOOLSET": "clang-6.0",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:6",
            "GROUP": "Linux",
        },
        "clang-5": {
            "NAME": "Clang 5",
            "EXE": "clang++-5.0",
            "TOOLSET": "clang-5.0",
            "CXXSTD": "11,14,17",
            "CONTAINER": "teeks99/clang-ubuntu:5",
            "GROUP": "Linux",
        },
        "clang-4": {
            "NAME": "Clang 4",
            "EXE": "clang++-4.0",
            "TOOLSET": "clang-4.0",
            "CXXSTD": "11,14",
            "CONTAINER": "teeks99/clang-ubuntu:4",
            "GROUP": "Linux",
        },
        "clang-3.9": {
            "NAME": "Clang 3.9",
            "EXE": "clang++-3.9",
            "TOOLSET": "clang-3.9",
            "CXXSTD": "11",
            "CONTAINER": "teeks99/clang-ubuntu:3.9",
            "GROUP": "Linux",
        },
        "tcc-0.9.27": {
            "NAME": "TCC 0.9.27",
            "EXE": "tcc",
            "TOOLSET": "cc",
            "B2_CONFIG": '<cflags>"-std=c11" <-soname>-soname',
            "CONTAINER": "ubuntu:24.04",
            "GROUP": "Linux",
            "EXCLUDE": True,
        },
    }

    def __init__(self):
        parser = argparse.ArgumentParser()
        parser.add_argument("--type")
        parser.add_argument("--include", default="")
        parser.add_argument("--exclude", default="")
        parser.add_argument("--group", default="")
        self.args = parser.parse_args()
        if self.args.type == "matrix":
            self.gen_matrix()

    def gen_matrix(self):
        include = set()
        if self.args.include:
            include = set(self.args.include.split(","))
        else:
            for mc in self.matrix_compilers.items():
                if "EXCLUDE" in mc[1] and mc[1]["EXCLUDE"]:
                    continue
                if self.args.group != mc[1]["GROUP"]:
                    continue
                include.add(mc[0])
        include -= set(self.args.exclude.split(","))
        result = {}
        for toolset in include:
            if toolset not in self.matrix_compilers:
                continue
            matrix_compiler = self.matrix_compilers[toolset]
            result[matrix_compiler["NAME"]] = matrix_compiler
        result_vso = "##vso[task.setVariable variable={0};isOutput=true]".format(
            self.args.group
        )
        result_vso += json.dumps(result, ensure_ascii=True, sort_keys=True, indent=None)
        print(json.dumps(result, ensure_ascii=True, sort_keys=True, indent=2))
        print(result_vso)


if __name__ == "__main__":
    CXXPipelines()

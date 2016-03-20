cmake_minimum_required(VERSION 2.8.11)

project(bf_asmjit)


set(CMAKE_BUILD_TYPE "Release")
set(DASMJIT_BUILD_TEST FALSE)
include(asmjit/CMakeLists.txt)

include(bf/CMakeLists.txt)

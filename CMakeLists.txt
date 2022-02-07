cmake_minimum_required(VERSION 3.13)

project(seele)
include(CTest)

set(CONAN_DISABLE_CHECK_COMPILER TRUE)

add_subdirectory(seele)
add_subdirectory(core)

cmake_minimum_required(VERSION 3.13)

project(seele)

set(CONAN_DISABLE_CHECK_COMPILER TRUE)
include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
find_package(GTest REQUIRED)

include(CTest)

add_subdirectory(seele)
add_subdirectory(core)

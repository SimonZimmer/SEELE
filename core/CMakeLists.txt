cmake_minimum_required(VERSION 3.13)

project(core)

add_library(core INTERFACE)

target_include_directories(core INTERFACE inc)

target_compile_features(core INTERFACE cxx_std_17)

enable_testing()
add_subdirectory(tests)
add_subdirectory(mocks)

cmake_minimum_required(VERSION 3.16)

project(pupilCore)

add_library(pupilCore STATIC
    src/DelayProcessor.cpp
    )

target_include_directories(pupilCore
    PUBLIC inc
    )

target_compile_features(pupilCore PUBLIC cxx_std_17)

add_subdirectory(tests)
add_subdirectory(mocks)

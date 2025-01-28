cmake_minimum_required(VERSION 3.13)

project(core)

add_library(core
    STATIC
        src/AudioBuffer.cpp
        src/InterpolatingValue.cpp
        )

target_include_directories(core
        PRIVATE
            inc/core
        PUBLIC
            inc
        )

target_compile_features(core PUBLIC cxx_std_17)

enable_testing()
add_subdirectory(tests)
add_subdirectory(mocks)

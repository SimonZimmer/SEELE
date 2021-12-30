cmake_minimum_required(VERSION 3.13)

project(seeleCore)

if (NOT TARGET juceaide)
    add_subdirectory(../ThirdParty/JUCE juce)
endif()

set(Python3_EXECUTABLE /usr/local/Cellar/python@3.9/3.9.9/bin/python3.9)
add_subdirectory(../../ThirdParty/matplotlibcpp matplotlibcpp)

add_library(seeleCore
        STATIC
        src/Factory.cpp
        src/Engine.cpp
        src/PhaseVocoder.cpp
        src/PhaseCorrector.cpp
    )

target_include_directories(seeleCore
    PRIVATE
        inc/seeleCore
    PUBLIC
        inc
        ../ThirdParty/matplotlibcpp
    )

target_link_libraries(seeleCore
   PUBLIC
        core
        juce::juce_core
        juce::juce_audio_basics
        juce::juce_dsp
        matplotlib_cpp
        )

target_compile_features(seeleCore PUBLIC cxx_std_17)

enable_testing()
add_subdirectory(tests)
add_subdirectory(mocks)

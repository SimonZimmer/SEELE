cmake_minimum_required(VERSION 3.16)

project(pupilCore)

if (NOT TARGET juceaide)
    add_subdirectory(../ThirdParty/JUCE juce)
endif()

add_library(pupilCore
        STATIC
        src/AmplitudeModulation.cpp
        src/Factory.cpp
        src/Engine.cpp
        src/SampleRamp.cpp
    )

target_include_directories(pupilCore
    PRIVATE
        inc/pupilCore
    PUBLIC
        inc
    )

target_link_libraries(pupilCore
   PUBLIC
        core
        juce::juce_core
        juce::juce_audio_basics
        juce::juce_dsp
        )

target_compile_features(pupilCore PUBLIC cxx_std_17)

add_subdirectory(tests)
add_subdirectory(mocks)

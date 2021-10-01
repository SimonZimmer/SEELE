cmake_minimum_required(VERSION 3.13)

project(seeleCore)

if (NOT TARGET juceaide)
    add_subdirectory(../ThirdParty/JUCE juce)
endif()

add_library(seeleCore
        STATIC
        src/AmplitudeModulation.cpp
        src/Factory.cpp
        src/Engine.cpp
        src/SampleRamp.cpp
        src/PhaseVocoder.cpp
    )

target_include_directories(seeleCore
    PRIVATE
        inc/seeleCore
    PUBLIC
        inc
    )

target_link_libraries(seeleCore
   PUBLIC
        core
        juce::juce_core
        juce::juce_audio_basics
        juce::juce_dsp
        )

target_compile_features(seeleCore PUBLIC cxx_std_17)

enable_testing()
add_subdirectory(tests)
add_subdirectory(mocks)

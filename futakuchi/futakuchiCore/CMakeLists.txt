cmake_minimum_required(VERSION 3.13)

project(futakuchiCore)

if (NOT TARGET juceaide)
    add_subdirectory(../ThirdParty/JUCE juce)
endif()

add_library(futakuchiCore
        STATIC
        src/AmplitudeModulation.cpp
        src/Factory.cpp
        src/Engine.cpp
        src/SampleRamp.cpp
    )

target_include_directories(futakuchiCore
    PRIVATE
        inc/futakuchiCore
    PUBLIC
        inc
    )

target_link_libraries(futakuchiCore
   PUBLIC
        core
        juce::juce_core
        juce::juce_audio_basics
        juce::juce_dsp
        )

target_compile_features(futakuchiCore PUBLIC cxx_std_17)

enable_testing()
add_subdirectory(tests)
add_subdirectory(mocks)

cmake_minimum_required(VERSION 3.10)
project(pupilPlugin)

if (NOT TARGET juceaide)
    add_subdirectory(../ThirdParty/JUCE juce)
endif()

add_subdirectory(pupilCore)

set(PLUGIN_NAME "Pupil")
set(FORMATS VST3 AU)
juce_add_plugin(pupilPlugin
        PLUGIN_MANUFACTURER_CODE SZ
        PLUGIN_CODE PPL
        FORMATS ${FORMATS}
        PRODUCT_NAME ${PLUGIN_NAME}
        COMPANY_NAME "Simon Zimmermann"
        VERSION 1.0)

target_compile_features(pupilPlugin PUBLIC cxx_std_17)

target_sources(pupilPlugin PRIVATE
        src/PluginEditor.cpp
        src/PluginProcessor.cpp
        )

target_include_directories(pupilPlugin PRIVATE inc)

target_compile_definitions(pupilPlugin PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0)

target_link_libraries(pupilPlugin
        PRIVATE
            juce::juce_core
            juce::juce_audio_utils
            pupilCore
        )

set(common_definitions
        JUCE_QUICKTIME=0
        JUCE_PLUGINHOST_AU=0
        JUCE_PLUGINHOST_AAX=0
        )

cmake_minimum_required(VERSION 3.13)
project(futakuchiPlugin)

if (NOT TARGET juceaide)
    add_subdirectory(../ThirdParty/JUCE juce)
endif()

add_subdirectory(futakuchiCore)

set(PLUGIN_NAME "Pupil")
set(FORMATS VST3 AU)
juce_add_plugin(futakuchiPlugin
        PLUGIN_MANUFACTURER_CODE SZ
        PLUGIN_CODE PPL
        FORMATS ${FORMATS}
        PRODUCT_NAME ${PLUGIN_NAME}
        COMPANY_NAME "Simon Zimmermann"
        VERSION 1.0)

target_compile_features(futakuchiPlugin PUBLIC cxx_std_17)

target_sources(futakuchiPlugin PRIVATE
        src/PluginEditor.cpp
        src/PluginProcessor.cpp
        )

target_include_directories(futakuchiPlugin PRIVATE inc)

target_compile_definitions(futakuchiPlugin PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0)

target_link_libraries(futakuchiPlugin
        PRIVATE
            juce::juce_core
            juce::juce_audio_utils
            futakuchiCore
        )

set(common_definitions
        JUCE_QUICKTIME=0
        JUCE_PLUGINHOST_AU=0
        JUCE_PLUGINHOST_AAX=0
        )

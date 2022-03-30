#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_graphics/juce_graphics.h>

#include "PluginProcessor.h"
#include "MainComponent.h"


class NewProjectAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    NewProjectAudioProcessor& processor;
    std::unique_ptr<hidonash::MainComponent> mainComponent_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};

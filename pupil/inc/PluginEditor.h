#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "PluginProcessor.h"


class NewProjectAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    NewProjectAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};

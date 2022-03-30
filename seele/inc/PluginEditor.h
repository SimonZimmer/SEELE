#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_graphics/juce_graphics.h>

#include "PluginProcessor.h"
#include "MainComponent.h"


class NewProjectAudioProcessorEditor : public juce::AudioProcessorEditor
                                     , private juce::Slider::Listener
{
public:
    explicit NewProjectAudioProcessorEditor (NewProjectAudioProcessor&);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void sliderValueChanged (juce::Slider* slider) override;

    NewProjectAudioProcessor& processor;
    std::unique_ptr<hidonash::MainComponent> mainComponent_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};

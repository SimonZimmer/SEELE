#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_graphics/juce_graphics.h>

#include "MainComponent.h"
#include "PluginProcessor.h"


class NewProjectAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit NewProjectAudioProcessorEditor(NewProjectAudioProcessor&);

    void resized() override;

private:
    std::unique_ptr<hidonash::MainComponent> mainComponent_;
    std::vector<std::unique_ptr<juce::SliderParameterAttachment>> sanctitySliderAttachments_;
    std::vector<std::unique_ptr<juce::SliderParameterAttachment>> distanceSliderAttachments_;
    std::vector<std::unique_ptr<juce::SliderParameterAttachment>> levelSliderAttachments_;
    std::vector<std::unique_ptr<juce::ButtonParameterAttachment>> buttonAttachments_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NewProjectAudioProcessorEditor)
};

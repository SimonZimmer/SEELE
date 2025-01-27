#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "PluginProcessor.h"


namespace hidonash
{
    class Oscilloscope : public juce::Component,
                         public juce::Timer
    {
    public:
        Oscilloscope(NewProjectAudioProcessor& processor, size_t numSamples,
                     int refreshRateHz = 30);

        ~Oscilloscope() override = default;

        void paint (juce::Graphics& g) override;

        void timerCallback() override;

    private:
        juce::Image scope_;
        NewProjectAudioProcessor& processor_;
        juce::AudioBuffer<float> displayBuffer_;
    };
}

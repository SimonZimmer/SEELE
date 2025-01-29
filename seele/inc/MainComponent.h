#pragma once

#include <array>
#include <memory>

#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>

#include "MemberArea.h"
#include "Oscilloscope.h"
#include "SummonToggle.h"


namespace hidonash
{
    class MainComponent : public juce::Component
    {
    public:
        MainComponent(NewProjectAudioProcessor& processor);

        ~MainComponent() override = default;

        void resized() override;

        void paint(juce::Graphics& g) override;

        SeeleSlider& getSanctitySlider(size_t index);

        SummonToggle& getSummonToggle(size_t index);

        MiniSlider& getDistanceSlider(size_t index);

        MiniSlider& getGainSlider(size_t index);

    private:
        std::unique_ptr<juce::Drawable> seeleLogo_;
        std::unique_ptr<MemberArea> memberArea_;
        std::array<std::unique_ptr<Oscilloscope>, 2> oscilloscope_;
        NewProjectAudioProcessor& processor_;
    };
}

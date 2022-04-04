#pragma once

#include <memory>

#include <juce_audio_processors/juce_audio_processors.h>

#include "MemberArea.h"
#include "SummonToggle.h"


namespace hidonash
{
    class MainComponent : public juce::Component
    {
    public:
        MainComponent();

        ~MainComponent() override = default;

        void resized() override;

        void paint(juce::Graphics& g) override;

        SeeleSlider& getSanctitySlider(size_t index);

        SummonToggle& getSummonToggle(size_t index);

    private:
        std::unique_ptr<juce::Drawable> seeleLogo_;
        std::unique_ptr<MemberArea> memberArea_;
    };
}

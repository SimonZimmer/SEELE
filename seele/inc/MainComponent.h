#pragma once

#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "SliderLookAndFeel.h"
#include "ToggleButtonLookAndFeel.h"
#include "TextBox.h"


namespace hidonash
{
    class MainComponent : public juce::Component
    {
    public:
        MainComponent();

        ~MainComponent() override = default;

        void paint(juce::Graphics& g) override;

        juce::Slider& getSanctitySlider(size_t index);

        juce::ToggleButton& getSummonToggle(size_t index);

    private:
        std::unique_ptr<juce::Drawable> seeleLogo_;
        std::vector<std::unique_ptr<SliderLookAndFeel>> sliderLookAndFeels_;
        std::vector<std::unique_ptr<juce::Slider>> sanctitySliders_;
        std::vector<std::unique_ptr<TextBox>> textBoxes_;
        std::vector<std::unique_ptr<ToggleButtonLookAndFeel>> toggleButtonLookAndFeels_;
        std::vector<std::unique_ptr<juce::ToggleButton>> summonToggles_;
        juce::GlowEffect glowEffect_{};
    };
}

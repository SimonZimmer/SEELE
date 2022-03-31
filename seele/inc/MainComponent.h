#pragma once

#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "TextBox.h"
#include "SeeleSlider.h"
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

        juce::ToggleButton& getSummonToggle(size_t index);

    private:
        std::unique_ptr<juce::Drawable> seeleLogo_;
        std::vector<std::unique_ptr<SeeleSlider>> sanctitySliders_;
        std::vector<std::unique_ptr<TextBox>> textBoxes_;
        std::vector<std::unique_ptr<SummonToggle>> toggleButtonLookAndFeels_;
        std::vector<std::unique_ptr<juce::ToggleButton>> summonToggles_;
        juce::String currentSizeAsString_;
    };
}

#pragma once

#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "TextBox.h"
#include "SeeleSlider.h"
#include "SummonToggle.h"


namespace hidonash
{
    class MemberArea : public juce::Component
    {
    public:
        MemberArea() = default;

        ~MemberArea() override = default;

        void resized() override;

        SeeleSlider& getSanctitySlider(size_t index);

        SummonToggle& getSummonToggle(size_t index);

    private:
        std::vector<std::unique_ptr<SeeleSlider>> sanctitySliders_;
        std::vector<std::unique_ptr<SummonToggle>> summonToggles_;
        std::vector<std::unique_ptr<TextBox>> textBoxes_;
    };
}


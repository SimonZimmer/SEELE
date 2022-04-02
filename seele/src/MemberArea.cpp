#include "MemberArea.h"

#include <seeleCore/Config.h>

#include "TextBox.h"
#include "MemberArea.h"


namespace hidonash
{
    MemberArea::MemberArea()
    {
    }

    void MemberArea::resized()
    {
        for(auto n = 0; n < config::constants::numMembers; ++n)
        {
            sanctitySliders_.emplace_back(std::make_unique<SeeleSlider>(n + 1));
            textBoxes_.emplace_back(std::make_unique<TextBox>(*sanctitySliders_[n])); summonToggles_.emplace_back(std::make_unique<SummonToggle>());

            const auto padding = getWidth() / 6.7f;

            {
                sanctitySliders_[n]->setBounds(padding * n, 0.f, getWidth() / 10.f, getHeight() / 1.1f);
                addAndMakeVisible(*sanctitySliders_[n]);
                textBoxes_[n]->setBounds(padding * n, (getHeight() / 1.25f), getWidth() / 10.f, getHeight() / 10.f);
                addAndMakeVisible(*textBoxes_[n]);
            }

            {
                summonToggles_[n]->setBounds(padding * n, getHeight() - getHeight() / 12.f, getWidth() / 10.f, 30.f);
                addAndMakeVisible(*summonToggles_[n]);
            }
        }
    }

    SeeleSlider& MemberArea::getSanctitySlider(size_t index)
    {
        return *sanctitySliders_[index];
    }

    juce::ToggleButton& MemberArea::getSummonToggle(size_t index)
    {
        return *summonToggles_[index];
    }
}


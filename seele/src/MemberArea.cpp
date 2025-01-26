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
        auto localBounds = getLocalBounds();
        const auto singleMemberWidth = getWidth() / config::constants::numMembers;
        const auto summonTogglesHeight = localBounds.getHeight() / 15.f;

        for(auto n = 0; n < config::constants::numMembers; ++n)
        {
            sanctitySliders_.emplace_back(std::make_unique<SeeleSlider>(n + 1));
            summonToggles_.emplace_back(std::make_unique<SummonToggle>());
            textBoxes_.emplace_back(std::make_unique<TextBox>(*sanctitySliders_[n]));

            auto singleMemberBounds = localBounds.removeFromLeft(singleMemberWidth);
            auto sliderBounds = (singleMemberBounds.removeFromLeft(singleMemberWidth / 2));
            sliderBounds.setHeight(localBounds.getHeight() - 2 * summonTogglesHeight);
            sanctitySliders_[n]->setBounds(sliderBounds);
            addAndMakeVisible(*sanctitySliders_[n]);

            auto summonToggleBounds = sliderBounds;
            summonToggleBounds.setTop(sliderBounds.getHeight() / 1.25);
            summonToggles_[n]->setBounds(summonToggleBounds);
            summonToggles_[n]->setTopLeftPosition(sliderBounds.getX(), sliderBounds.getY() + sliderBounds.getHeight() + summonTogglesHeight);
            addAndMakeVisible(*summonToggles_[n]);

            auto textBoxBounds = summonToggleBounds;
            textBoxBounds.setY(textBoxBounds.getY() - summonTogglesHeight);
            textBoxes_[n]->setBounds(textBoxBounds);
            addAndMakeVisible(*textBoxes_[n]);
        }
    }

    SeeleSlider& MemberArea::getSanctitySlider(size_t index)
    {
        return *sanctitySliders_[index];
    }

    SummonToggle& MemberArea::getSummonToggle(size_t index)
    {
        return *summonToggles_[index];
    }
}


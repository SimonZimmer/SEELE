#include "MemberArea.h"

#include <seeleCore/Config.h>

#include "TextBox.h"
#include "MemberArea.h"


namespace hidonash
{
    void MemberArea::resized()
    {
        auto localBounds = getLocalBounds();
        const auto singleMemberWidth = getWidth() / config::constants::numMembers;
        const auto summonTogglesHeight = localBounds.getHeight() / 15.f;

        for(auto n = 0; n < config::constants::numMembers; ++n)
        {
            sanctitySliders_.emplace_back(std::make_unique<SeeleSlider>(n + 1));
            auto singleMemberBounds = localBounds.removeFromLeft(singleMemberWidth);
            auto sliderBounds = singleMemberBounds.removeFromLeft(singleMemberWidth);
            const int widthPadding = singleMemberWidth / 6;
            sliderBounds.reduce(widthPadding, 0);
            sliderBounds.setHeight(localBounds.getHeight() - 4 * summonTogglesHeight);
            sanctitySliders_[n]->setBounds(sliderBounds);
            addAndMakeVisible(*sanctitySliders_[n]);

            delaySliders_.emplace_back(std::make_unique<MiniSlider>());
            auto delaySliderBounds = sliderBounds;
            delaySliderBounds.removeFromBottom(sliderBounds.getHeight() - summonTogglesHeight);
            delaySliders_[n]->setBounds(delaySliderBounds);
            delaySliders_[n]->setTopLeftPosition(sliderBounds.getX(), sliderBounds.getY() + sliderBounds.getHeight() + summonTogglesHeight);
            addAndMakeVisible(*delaySliders_[n]);

            textBoxes_.emplace_back(std::make_unique<TextBox>(*delaySliders_[n]));
            auto textBoxBounds = delaySliderBounds;
            const auto textBoxHeight = delaySliderBounds.getHeight();
            textBoxes_[n]->setBounds(delaySliderBounds);
            textBoxes_[n]->setTopLeftPosition(sliderBounds.getX(), sliderBounds.getY() + sliderBounds.getHeight() + 1.5 * summonTogglesHeight);
            addAndMakeVisible(*textBoxes_[n]);

            summonToggles_.emplace_back(std::make_unique<SummonToggle>());
            auto summonToggleBounds = sliderBounds;
            summonToggles_[n]->setBounds(summonToggleBounds);
            summonToggles_[n]->setTopLeftPosition(sliderBounds.getX(), sliderBounds.getY() + sliderBounds.getHeight() + 3 * summonTogglesHeight);
            addAndMakeVisible(*summonToggles_[n]);
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

    MiniSlider& MemberArea::getDelaySlider(size_t index)
    {
        return *delaySliders_[index];
    }

    MiniSlider& MemberArea::getPanSlider(size_t index)
    {
        return *panSliders_[index];
    }
}


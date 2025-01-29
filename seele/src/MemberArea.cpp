#include "MemberArea.h"

#include <seeleCore/Config.h>

#include "MemberArea.h"
#include "TextBox.h"


namespace hidonash
{
    void MemberArea::resized()
    {
        auto localBounds = getLocalBounds();
        const auto singleMemberWidth = getWidth() / config::constants::numMembers;
        const auto summonTogglesHeight = localBounds.getHeight() / 15.f;

        for (auto n = 0; n < config::constants::numMembers; ++n)
        {
            sanctitySliders_.emplace_back(std::make_unique<SeeleSlider>(n + 1));
            auto singleMemberBounds = localBounds.removeFromLeft(singleMemberWidth);
            auto sliderBounds = singleMemberBounds.removeFromLeft(singleMemberWidth);
            const int widthPadding = singleMemberWidth / 6;
            sliderBounds.reduce(widthPadding, 0);
            sliderBounds.setHeight(localBounds.getHeight() - 4 * summonTogglesHeight);
            sanctitySliders_[n]->setBounds(sliderBounds);
            addAndMakeVisible(*sanctitySliders_[n]);

            distanceSliders_.emplace_back(std::make_unique<MiniSlider>());
            auto distanceSliderBounds = sliderBounds;
            distanceSliderBounds.removeFromBottom(sliderBounds.getHeight() - summonTogglesHeight);
            distanceSliders_[n]->setBounds(distanceSliderBounds);
            distanceSliders_[n]->setTopLeftPosition(
                sliderBounds.getX(), sliderBounds.getY() + sliderBounds.getHeight() + summonTogglesHeight);
            addAndMakeVisible(*distanceSliders_[n]);

            /*
            textBoxes_.emplace_back(std::make_unique<TextBox>(*distanceSliders_[n]));
            auto textBoxBounds = distanceSliderBounds;
            const auto textBoxHeight = distanceSliderBounds.getHeight();
            textBoxes_[n]->setBounds(distanceSliderBounds);
            textBoxes_[n]->setTopLeftPosition(sliderBounds.getX(), sliderBounds.getY() + sliderBounds.getHeight() +
            addAndMakeVisible(*textBoxes_[n]);
            */

            summonToggles_.emplace_back(std::make_unique<SummonToggle>());
            auto summonToggleBounds = sliderBounds;
            summonToggles_[n]->setBounds(summonToggleBounds);
            summonToggles_[n]->setTopLeftPosition(sliderBounds.getX(), sliderBounds.getY() + sliderBounds.getHeight() +
                                                                           3 * summonTogglesHeight);
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

    MiniSlider& MemberArea::getDistanceSlider(size_t index)
    {
        return *distanceSliders_[index];
    }

    MiniSlider& MemberArea::getPanSlider(size_t index)
    {
        return *panSliders_[index];
    }
}

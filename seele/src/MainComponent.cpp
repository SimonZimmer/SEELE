#include "MainComponent.h"

#include "juce_gui_basics/juce_gui_basics.h"

#include "MemberArea.h"
#include "Oscilloscope.h"

#include <seeleCore/Config.h>
#include <GraphicAssets.h>


namespace hidonash
{
    MainComponent::MainComponent(NewProjectAudioProcessor& processor)
    : seeleLogo_(juce::Drawable::createFromImageData(hidonash::resources::graphicassets::seele_frame_png,
                                                     hidonash::resources::graphicassets::seele_frame_pngSize))
    , processor_(processor)
    {
        memberArea_ = std::make_unique<hidonash::MemberArea>();
        addAndMakeVisible(memberArea_.get());

        for (auto n = 0; n < oscilloscope_.size(); ++n)
        {
            oscilloscope_[n] = std::make_unique<hidonash::Oscilloscope>(processor_, processor_.getBlockSize());
            addAndMakeVisible(oscilloscope_[n].get());
        }
    }

    void MainComponent::resized()
    {
        const auto widthPadding = getWidth() / 15.f;
        const auto heightPadding = getHeight() / 8.f;

        auto memberBounds = getBounds().reduced(widthPadding, heightPadding);
        memberArea_->setBounds(memberBounds);
        memberArea_->setTopLeftPosition(widthPadding, 1.25f * heightPadding);

        auto oscilloscopeBoundsLeft = getBounds();
        oscilloscopeBoundsLeft.removeFromLeft(getWidth() * 0.774f);
        oscilloscopeBoundsLeft.removeFromBottom(getHeight() * 0.917f);
        oscilloscopeBoundsLeft.removeFromRight(getWidth() * 0.138f);
        oscilloscopeBoundsLeft.removeFromTop(getHeight() * 0.045f);
        oscilloscope_[0]->setBounds(oscilloscopeBoundsLeft);

        auto oscilloscopeBoundsRight = oscilloscopeBoundsLeft;
        oscilloscope_[1]->setBounds(oscilloscopeBoundsRight);
        oscilloscope_[1]->setTopLeftPosition(oscilloscopeBoundsLeft.getX() + getWidth() / 10.f, oscilloscopeBoundsLeft.getY());
    }

    SeeleSlider& MainComponent::getSanctitySlider(size_t index)
    {
        return memberArea_->getSanctitySlider(index); 
    }

    SummonToggle& MainComponent::getSummonToggle(size_t index)
    {
        return memberArea_->getSummonToggle(index); 
    }

    void MainComponent::paint(juce::Graphics& g)
    {
        g.fillAll(juce::Colour::greyLevel(0.f));
        g.setColour (juce::Colours::red);

        seeleLogo_->drawWithin(g, getBounds().toFloat(), juce::RectanglePlacement::centred, 1.f);
    }
}

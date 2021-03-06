#include "MainComponent.h"
#include "MemberArea.h"

#include <GraphicAssets.h>


namespace hidonash
{
    MainComponent::MainComponent()
    : seeleLogo_(juce::Drawable::createFromImageData(hidonash::resources::graphicassets::seele_frame_svg,
                                                     hidonash::resources::graphicassets::seele_frame_svgSize))
    {
        memberArea_ = std::make_unique<hidonash::MemberArea>();
        addAndMakeVisible(memberArea_.get());
    }

    void MainComponent::resized()
    {
        const auto widthPadding = getWidth() / 7.f;
        const auto heightPadding = getHeight() / 7.f;
        auto memberBounds = getBounds().reduced(widthPadding, heightPadding);
        memberArea_->setBounds(memberBounds);
        memberArea_->setTopLeftPosition(widthPadding, heightPadding * 1.3f);
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


#include "MainComponent.h"

#include <seeleCore/Config.h>
#include <GraphicAssets.h>


namespace hidonash
{
    MainComponent::MainComponent()
    : seeleLogo_(juce::Drawable::createFromImageData(hidonash::resources::graphicassets::seele_logo_svg,
                                                     hidonash::resources::graphicassets::seele_logo_svgSize))
    {
        setSize(465, 520);

        for(auto n = 0; n < config::constants::numMembers; ++n)
        {
            sanctitySliders_.emplace_back(std::make_unique<juce::Slider>(juce::Slider::LinearBarVertical,
                                                                         juce::Slider::TextEntryBoxPosition::TextBoxAbove));
            summonToggles_.emplace_back(std::make_unique<juce::ToggleButton>());
            toggleButtonLookAndFeels_.emplace_back(std::make_unique<ToggleButtonLookAndFeel>());

            sliderLookAndFeels_.emplace_back(std::make_unique<SliderLookAndFeel>(n + 1));

            glowEffect_.setGlowProperties(3, Colour::fromRGBA(170, 160, 240, 240));

            {
                sanctitySliders_[n]->setBounds(10 + (65 * n), 230, 55, 220);
                sanctitySliders_[n]->setRange(0, 100, 0.01f);
                sanctitySliders_[n]->setDoubleClickReturnValue(true, config::parameters::defaultPitchFactor);
                sanctitySliders_[n]->setTooltip("Sanctity of the Seele Member");
                sanctitySliders_[n]->setLookAndFeel(sliderLookAndFeels_[n].get());
                addAndMakeVisible(*sanctitySliders_[n]);
            }

            {
                summonToggles_[n]->setComponentEffect(&glowEffect_);
                summonToggles_[n]->setBounds(10 + (65 * n), 450, 55, 25);
                sanctitySliders_[n]->setTooltip("Summon the Seele Member");
                summonToggles_[n]->setLookAndFeel(toggleButtonLookAndFeels_[n].get());
                addAndMakeVisible(*summonToggles_[n]);
            }
        }
    }

    juce::Slider& MainComponent::getSanctitySlider(size_t index)
    {
        return *sanctitySliders_[index];
    }

    juce::ToggleButton& MainComponent::getSummonToggle(size_t index)
    {
        return *summonToggles_[index];
    }

    void MainComponent::paint(juce::Graphics& g)
    {
        g.fillAll(juce::Colour::greyLevel(0.f));
        g.setColour (juce::Colours::red);

        auto logoBounds = juce::Rectangle<float>(200, 200);
        logoBounds.setPosition((getWidth() / 2) - (logoBounds.getWidth() / 2), 10);
        seeleLogo_->drawWithin(g, logoBounds, juce::RectanglePlacement::centred, 1.f);
    }
}


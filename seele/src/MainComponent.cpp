#include "MainComponent.h"
#include "TextBox.h"

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
            sanctitySliders_.emplace_back(std::make_unique<SeeleSlider>(n + 1));
            textBoxes_.emplace_back(std::make_unique<TextBox>(*sanctitySliders_[n])); summonToggles_.emplace_back(std::make_unique<SummonToggle>());

            {
                sanctitySliders_[n]->setBounds(10 + (65 * n), 230, 55, 220);
                addAndMakeVisible(*sanctitySliders_[n]);
                textBoxes_[n]->setBounds(10 + (65 * n), 425, 50, 30 );
                addAndMakeVisible(*textBoxes_[n]);
            }

            {
                summonToggles_[n]->setBounds(10 + (65 * n), 450, 55, 25);
                addAndMakeVisible(*summonToggles_[n]);
            }
        }
    }

    SeeleSlider& MainComponent::getSanctitySlider(size_t index)
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

        auto logoBounds = juce::Rectangle<float>(getWidth() / 2.f, getWidth() / 2.f);
        logoBounds.setPosition((getWidth() / 2.f) - (logoBounds.getWidth() / 2.f), 10);
        seeleLogo_->drawWithin(g, logoBounds, juce::RectanglePlacement::centred, 1.f);
    }
}


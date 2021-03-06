#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include <GraphicAssets.h>


namespace hidonash
{
    class ToggleButtonLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        ToggleButtonLookAndFeel()
        {
            setColour(juce::ToggleButton::ColourIds::textColourId, juce::Colours::transparentBlack); 
        }

        void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button, bool, bool) override
        {
            const auto enabledColour = juce::Colour::fromRGBA(255, 245, 255, 255);
            const auto disabledColour = juce::Colour::fromRGBA(255, 245, 255, 30);

            if(button.getToggleState())
                g.setColour(enabledColour);
            else
                g.setColour(disabledColour);

            const auto buttonBounds = button.getLocalBounds().reduced(2, 10).toFloat();
            g.fillRect(buttonBounds);

            static const auto font = juce::Typeface::createSystemTypefaceFor(resources::graphicassets::ChicagoFLF_ttf,
                                                                             resources::graphicassets::ChicagoFLF_ttfSize);
            g.setFont(font);
            g.setFont(10.f);
        }
    };

    class SummonToggle : public juce::ToggleButton
    {
    public:
        SummonToggle()
        {
            setLookAndFeel(&lookAndFeel_);

            glowEffect_.setGlowProperties(3, juce::Colour::fromRGBA(170, 160, 240, 240));
            setComponentEffect(&glowEffect_);
        }

        void mouseEnter(const juce::MouseEvent&) override
        {
            setMouseCursor(juce::MouseCursor::StandardCursorType::PointingHandCursor);
        }

        void mouseExit(const juce::MouseEvent &) override
        {
            setMouseCursor(juce::MouseCursor::StandardCursorType::NormalCursor);
        }

    private:
        ToggleButtonLookAndFeel lookAndFeel_{};
        juce::GlowEffect glowEffect_{};
    };
}


#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <GraphicAssets.h>


namespace hidonash
{
    using namespace juce;

    class ToggleButtonLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        ToggleButtonLookAndFeel() = default;

        void drawToggleButton(Graphics& g, ToggleButton& button, bool, bool) override
        {
            const auto enabledColour = Colour::fromRGBA(255, 245, 255, 255);
            const auto disabledColour = Colour::fromRGBA(255, 245, 255, 30);

            if (button.getToggleState())
                g.setColour(enabledColour);
            else
                g.setColour(disabledColour);

            g.fillRect(button.getLocalBounds().reduced(2, 10).toFloat());

            static const auto font = juce::Typeface::createSystemTypefaceFor(resources::graphicassets::ChicagoFLF_ttf,
                                                                             resources::graphicassets::ChicagoFLF_ttfSize);
            g.setFont(font);
            g.setFont(10.f);
        }
    };
}

#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


namespace hidonash
{
    class ToggleButtonLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        ToggleButtonLookAndFeel() = default;

        void drawToggleButton(Graphics& g, ToggleButton& button, bool, bool) override
        {
            const auto enabledColour = Colour::fromRGBA(216, 216, 230, 255);
            const auto disabledColour = Colour::fromRGBA(216, 216, 230, 50);

            if (button.getToggleState())
                g.setColour(enabledColour);
            else
                g.setColour(disabledColour);

            g.fillRect(button.getLocalBounds());
        }
    };
}

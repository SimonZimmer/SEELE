#pragma once

#include <juce_gui_basics/juce_gui_basics.h>


namespace hidonash
{
    class Font
    {
    public:
        static juce::Font chicagoFLF()
        {
            static const auto font = juce::Typeface::createSystemTypefaceFor(resources::graphicassets::ChicagoFLF_ttf,
                                                                             resources::graphicassets::ChicagoFLF_ttfSize);
            return font;
        }
    };
}


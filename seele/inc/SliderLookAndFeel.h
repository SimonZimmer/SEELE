#include <juce_gui_basics/juce_gui_basics.h>

#include <GraphicAssets.h>


namespace hidonash
{
    using namespace juce;

    class SliderLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        SliderLookAndFeel(int memberNumber)
        : memberIdentifier_("0" + std::to_string(memberNumber))
        {
        }

        void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                               float sliderPos, float minSliderPos, float maxSliderPos,
                               const Slider::SliderStyle style, Slider& slider) override
        {
            g.fillAll(Colour::fromRGB(40, 40, 70));

            const float fx = (float) x, fy = (float) y, fw = (float) width, fh = (float) height;

            Path p;

            if (style == Slider::LinearBarVertical)
                p.addRectangle (fx, sliderPos, fw, 1.0f + fh - sliderPos);
            else
                p.addRectangle (fx, fy, sliderPos - fx, fh);

            auto baseColour = Colour::fromRGB(110, 104, 162)
                                    .withMultipliedSaturation (slider.isEnabled() ? 1.0f : 0.5f)
                                    .withMultipliedAlpha (0.8f);

            g.setGradientFill (ColourGradient::vertical (baseColour.brighter (0.08f), 0.0f,
                                                         baseColour.darker (0.08f), (float) height));
            g.fillPath(p);
            g.setColour(baseColour.darker (0.2f));
            g.fillRect(fx, sliderPos, fw, 1.0f);

            g.setColour(Colours::red);

            static const auto font = juce::Typeface::createSystemTypefaceFor(resources::graphicassets::ChicagoFLF_ttf,
                                                                             resources::graphicassets::ChicagoFLF_ttfSize);
            g.setFont(font);
            g.setFont(10.f);
            g.drawMultiLineText("SEELE\n\n\nSOUND\nONLY", width * 0.1f, height * 0.1, width * 0.9, Justification::centred);

            g.setFont(20.0f);
            g.drawText(memberIdentifier_, Rectangle<int>(width * 0.1f, height * 0.055, width * 0.9, height * 0.2), Justification::centred);
        }

    private:
        const std::string memberIdentifier_;
    };
}


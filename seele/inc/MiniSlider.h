#include "juce_graphics/juce_graphics.h"
#include <juce_gui_basics/juce_gui_basics.h>

#include <GraphicAssets.h>
#include <Font.h>
#include <seeleCore/Config.h>


namespace hidonash
{
    namespace
    {
        class MiniSliderLookAndFeel : public juce::LookAndFeel_V4
        {
        public:
            MiniSliderLookAndFeel()
            {
                setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
            }

            void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                  float sliderPos, float minSliderPos, float maxSliderPos,
                                  const juce::Slider::SliderStyle style, juce::Slider& slider) override
            {
                const auto sliderWidth = slider.getWidth();
                g.setColour(juce::Colours::red);

                auto bigBarPath = juce::Path();
                bigBarPath.addRectangle(x, y, 1.0f + sliderPos, 0.4f * height);

                auto smallBarPath = juce::Path();
                smallBarPath.addRectangle(x, y + 0.5f * height, 1.0f + width, height * 0.1f);

                auto textBounds = juce::Rectangle<int>(x, y + 0.7f * height, 1.0f + width, height * 0.3f);

                g.fillPath(bigBarPath);
                g.fillPath(smallBarPath);
                g.setFont(Font::chicagoFLF());
                g.setFont(sliderWidth * 0.125f);
                g.drawText("DISTANCE", textBounds, juce::Justification::left);
            }
        };
    }

    class MiniSlider : public juce::Slider
    {
    public:
        MiniSlider()
        : lookAndFeel_()
        {
            setSliderStyle(juce::Slider::LinearBar);
            setTextBoxStyle(NoTextBox, false, 0, 0);
            setDoubleClickReturnValue(true, config::parameters::minDistanceInSamples);
            setRange(0.f, 100.f, 0.01f);
            setTooltip("Distance (delay in ms) of the SEELE Member");
            setLookAndFeel(&lookAndFeel_);
        }

        void mouseEnter(const juce::MouseEvent& e) override
        {
            setMouseCursor(juce::MouseCursor::StandardCursorType::PointingHandCursor);
        }

        void mouseExit(const juce::MouseEvent &) override
        {
            setMouseCursor(juce::MouseCursor::StandardCursorType::NormalCursor);
        }

    private:
        MiniSliderLookAndFeel lookAndFeel_;
    };
}


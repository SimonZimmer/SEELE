#include <juce_gui_basics/juce_gui_basics.h>

#include <GraphicAssets.h>
#include <Font.h>
#include <seeleCore/Config.h>


namespace hidonash
{
    namespace
    {
        class SliderLookAndFeel : public juce::LookAndFeel_V4
        {
        public:
            SliderLookAndFeel(int memberNumber)
            : memberIdentifier_("0" + std::to_string(memberNumber))
            {
                setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack);
            }

            void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                  float sliderPos, float minSliderPos, float maxSliderPos,
                                  const juce::Slider::SliderStyle style, juce::Slider& slider) override
            {
                g.fillAll(juce::Colour::fromRGB(40, 40, 70));
                const auto sliderWidth = float(slider.getWidth());

                auto path = juce::Path();
                path.addRectangle(static_cast<float>(x), sliderPos, static_cast<float>(height), 1.f + static_cast<float>(height) - sliderPos);
                const auto baseColour = juce::Colour::fromRGB(110, 104, 162)
                                        .withMultipliedSaturation(slider.isEnabled() ? 1.f : 0.5f)
                                        .withMultipliedAlpha(0.8f);
                g.setGradientFill(juce::ColourGradient::vertical(baseColour.brighter(0.2f), 0.f,
                                                                 baseColour.darker(1.f), static_cast<float>(height)));
                g.fillPath(path);
                g.setColour(baseColour.darker (0.2f));
                g.fillRect(static_cast<float>(x), sliderPos, sliderWidth, 1.f);

                g.setColour(juce::Colours::red);
                g.setFont(Font::chicagoFLF());
                g.setFont(sliderWidth / 5.f);
                const auto sliderPosX = static_cast<int>(sliderWidth * 0.1);
                const auto sliderPosY = static_cast<int>(height * 0.1);
                const auto elementWidth = static_cast<int>(sliderWidth * 0.9);
                const auto elementHeight = static_cast<int>(height * 0.2);
                g.drawMultiLineText("SEELE\n\n\n\nSOUND\nONLY", sliderPosX, sliderPosY, elementWidth, juce::Justification::centred);
                g.drawText("SANCTITY", juce::Rectangle<int>(sliderPosX, height * 0.8, elementWidth, elementHeight), juce::Justification::centred);
                g.setFont(sliderWidth / 2.5f);
                g.drawText(memberIdentifier_, juce::Rectangle<int>(sliderPosX, height * 0.055, elementWidth, elementHeight), juce::Justification::centred);
            }

        private:
            const std::string memberIdentifier_;
        };
    }

    class SeeleSlider : public juce::Slider
    {
    public:
        SeeleSlider(int memberNumber)
        : lookAndFeel_(memberNumber)
        {
            setSliderStyle(juce::Slider::LinearBarVertical);
            setTextBoxStyle(NoTextBox, false, 0, 0);
            setDoubleClickReturnValue(true, config::parameters::defaultPitchFactor);
            setRange(0.f, 1.f, 0.01f);
            setLookAndFeel(&lookAndFeel_);
        }

        void mouseEnter(const juce::MouseEvent& e) override
        {
            setMouseCursor(juce::MouseCursor::StandardCursorType::PointingHandCursor);
            setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colour::greyLevel(0.5f)); 
        }

        void mouseExit(const juce::MouseEvent &) override
        {
            setMouseCursor(juce::MouseCursor::StandardCursorType::NormalCursor);
            setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::transparentBlack); 
        }

    private:
        SliderLookAndFeel lookAndFeel_;
    };
}


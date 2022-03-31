#pragma once


#include <juce_gui_basics/juce_gui_basics.h>

#include <GraphicAssets.h>


namespace hidonash 
{
    class TextBox : public juce::Component,
                    public juce::Slider::Listener
    {
    public:
        explicit TextBox(juce::Slider& slider)
        : slider_(slider)
        {
            static const auto font = juce::Font(juce::Typeface::createSystemTypefaceFor(resources::graphicassets::ChicagoFLF_ttf,
                                                                                        resources::graphicassets::ChicagoFLF_ttfSize));
            label_.setFont(font.withHeight(10.f));
            label_.setJustificationType(juce::Justification::centred);
            label_.setEditable(true, true, false);
            label_.setColour(juce::Label::backgroundColourId, juce::Colours::transparentBlack);
            label_.setColour(juce::Label::outlineColourId, juce::Colours::transparentBlack);
            label_.setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
            label_.setColour(juce::TextEditor::textColourId, juce::Colours::red);
            label_.setColour(juce::Label::ColourIds::textColourId, juce::Colours::red);

            updateFromSlider();

            label_.onEditorShow = [this] {label_.getCurrentTextEditor()->setInputRestrictions(0, "-.0123456789");};

            label_.onTextChange = [this]
            {
                const auto value = slider_.getRange().clipValue(label_.getText(false).getDoubleValue());

                if (slider_.getValue() != value)
                    slider_.setValue(value, juce::sendNotificationSync);
                else
                    updateFromSlider();
            };

            slider_.addListener(this);

            addAndMakeVisible(label_);
        }

        ~TextBox() override
        {
            try
            {
                slider_.removeListener(this);
            }
            catch (...)
            {}
        }

        void resized() override
        {
            label_.setBounds(getLocalBounds());
        }

    private:
        void sliderValueChanged(juce::Slider* slider) override
        {
            if (slider != &slider_)
                return;

            updateFromSlider();
        }

        void updateFromSlider()
        {
            label_.setText(juce::String(slider_.getValue(), 2), juce::dontSendNotification);
        }

        juce::Slider& slider_;
        juce::Label label_;
    };

}

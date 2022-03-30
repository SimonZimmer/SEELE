#pragma once

#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>


namespace hidonash
{
    class MainComponent : public juce::Component,
                          public juce::Slider::Listener
    {
    public:
        MainComponent();

        ~MainComponent() override = default;

        void paint(juce::Graphics& g) override;

        void sliderValueChanged (juce::Slider* slider) override;

    private:
        std::unique_ptr<juce::Drawable> seeleLogo_;
        std::vector<std::unique_ptr<juce::Slider>> sanctitySliders_;
    };
}

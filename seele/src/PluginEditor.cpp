#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "juce_audio_processors/juce_audio_processors.h"

#include <memory>
#include <seeleCore/Config.h>


NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor(NewProjectAudioProcessor& p)
: AudioProcessorEditor(&p)
{
    mainComponent_ = std::make_unique<hidonash::MainComponent>();
    setSize(700, 500);
    addAndMakeVisible(mainComponent_.get());

    setResizable(true, true);
    const auto ratio = 1.4f;
    auto&& constrainer = getConstrainer();
    constrainer->setFixedAspectRatio(ratio);
    constrainer->setMinimumSize(350, 250);

    for (auto n = 0; n < hidonash::config::constants::numMembers; ++n)
    {
        const auto sanctityParameterID = hidonash::config::parameters::sanctityPrefix + std::to_string(n);
        sanctitySliderAttachments_.emplace_back(std::make_unique<juce::SliderParameterAttachment>(
            *(p.getAudioProcessorValueTreeState().getParameter(sanctityParameterID)),
            mainComponent_->getSanctitySlider(n)));

        const auto summonStateparameterID = hidonash::config::parameters::summonStatePrefix + std::to_string(n);
        buttonAttachments_.emplace_back(std::make_unique<juce::ButtonParameterAttachment>(
            *(p.getAudioProcessorValueTreeState().getParameter(summonStateparameterID)),
            mainComponent_->getSummonToggle(n)));

        const auto distanceParameterID = hidonash::config::parameters::distancePrefix + std::to_string(n);
        distanceSliderAttachments_.emplace_back(std::make_unique<juce::SliderParameterAttachment>(
            *(p.getAudioProcessorValueTreeState().getParameter(distanceParameterID)),
            mainComponent_->getDistanceSlider(n)));

        const auto levelParameterID = hidonash::config::parameters::levelPrefix + std::to_string(n);
        levelSliderAttachments_.emplace_back(std::make_unique<juce::SliderParameterAttachment>(
            *(p.getAudioProcessorValueTreeState().getParameter(levelParameterID)), mainComponent_->getGainSlider(n)));
    }
}

void NewProjectAudioProcessorEditor::resized()
{
    mainComponent_->setBounds(0, 0, getWidth(), getHeight());
}

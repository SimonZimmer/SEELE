#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <memory>
#include <seeleCore/Config.h>


NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    mainComponent_ = std::make_unique<hidonash::MainComponent>();
    setSize(mainComponent_->getWidth(), mainComponent_->getHeight());
    addAndMakeVisible(mainComponent_.get());

    for(auto n = 0; n < hidonash::config::constants::numMembers; ++n)
    {
        const auto sanctityParameterID = hidonash::config::parameters::sanctityPrefix + std::to_string(n);
        sliderAttachments_.emplace_back(std::make_unique<juce::SliderParameterAttachment>(*(p.getAudioProcessorValueTreeState().getParameter(sanctityParameterID)),
                                                                                          mainComponent_->getSanctitySlider(n)));

        const auto summonStateparameterID = hidonash::config::parameters::summonStatePrefix + std::to_string(n);
        buttonAttachments_.emplace_back(std::make_unique<juce::ButtonParameterAttachment>(*(p.getAudioProcessorValueTreeState().getParameter(summonStateparameterID)),
                                                                                          mainComponent_->getSummonToggle(n)));
    }

}

void NewProjectAudioProcessorEditor::paint (juce::Graphics& g) {}

void NewProjectAudioProcessorEditor::resized()
{
    mainComponent_->setBounds(0, 0, mainComponent_->getWidth(), mainComponent_->getHeight());
}


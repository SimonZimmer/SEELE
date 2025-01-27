#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <memory>
#include <seeleCore/Config.h>


NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
: AudioProcessorEditor(&p), processor_ (p)
{
    mainComponent_ = std::make_unique<hidonash::MainComponent>(processor_);
    setSize(640, 380);
    addAndMakeVisible(mainComponent_.get());

    setResizable(true, true);
    const auto ratio = 1.7;
    auto&& constrainer = getConstrainer();
    constrainer->setFixedAspectRatio(ratio);
    constrainer->setMinimumSize(320, 190);

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

void NewProjectAudioProcessorEditor::resized()
{
    mainComponent_->setBounds(0, 0, getWidth(), getHeight());
}


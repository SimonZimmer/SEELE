#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <seeleCore/Config.h>


NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    mainComponent_ = std::make_unique<hidonash::MainComponent>();
    setSize(mainComponent_->getWidth(), mainComponent_->getHeight());
    addAndMakeVisible(mainComponent_.get());

    for(auto n = 0; n < hidonash::config::constants::numMembers; ++n)
        mainComponent_->getSanctitySlider(n).addListener(this);
}

void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void NewProjectAudioProcessorEditor::resized()
{
    mainComponent_->setBounds(0, 0, mainComponent_->getWidth(), mainComponent_->getHeight());

}

void NewProjectAudioProcessorEditor::sliderValueChanged (juce::Slider* slider)
{
    for(auto n = 0; n < hidonash::config::constants::numMembers; ++n)
        if (processor.getParameters()[n]->getName(256) == slider->getComponentID())
            processor.getParameters()[n]->setValue(slider->getValue());
}

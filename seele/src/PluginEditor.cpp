#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <seeleCore/Config.h>


NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    mainComponent_ = std::make_unique<hidonash::MainComponent>();
    setSize(mainComponent_->getWidth(), mainComponent_->getHeight());
    addAndMakeVisible(mainComponent_.get());
}

void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void NewProjectAudioProcessorEditor::resized()
{
    mainComponent_->setBounds(0, 0, mainComponent_->getWidth(), mainComponent_->getHeight());
}

#include "PluginProcessor.h"
#include "PluginEditor.h"


NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (NewProjectAudioProcessor& p)
        : AudioProcessorEditor (&p), processor (p)
{
    setSize (400, 300);
}

void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
}

void NewProjectAudioProcessorEditor::resized()
{
}

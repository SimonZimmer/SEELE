#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <pupilCore/Factory.h>

NewProjectAudioProcessor::NewProjectAudioProcessor()
: AudioProcessor (BusesProperties().withInput("Input",  juce::AudioChannelSet::stereo(), true)
                                   .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    parameters_(*this, nullptr, juce::Identifier ("mockPlugin"),
        {
            std::make_unique<juce::AudioParameterFloat>("chopLength",
                                                "ChopLength",
                                                juce::NormalisableRange<float>(0.0f, 1.0f),
                                                0.5f,
                                                "",
                                                juce::AudioProcessorParameter::genericParameter),
            std::make_unique<juce::AudioParameterFloat>("chopGain",
                                                "ChopGain",
                                                juce::NormalisableRange<float>(0.0f, 1.f),
                                                0.5f,
                                                "dB",
                                                juce::AudioProcessorParameter::genericParameter,
                                                [](float value, int index)
                                                {
                                                    return juce::String(value, 1) + " dB";
                                                })
        })
        , engine_(sz::Factory().createEngine(*parameters_.getRawParameterValue("chopLength"),
                                             *parameters_.getRawParameterValue("chopGain")))
{
    setLatencySamples(latency_);

    programs_.emplace_back("program one");
    programs_.emplace_back("program two");
    programs_.emplace_back("program three");
}

const juce::String NewProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NewProjectAudioProcessor::acceptsMidi() const
{
    return false;
}

bool NewProjectAudioProcessor::producesMidi() const
{
    return false;
}

bool NewProjectAudioProcessor::isMidiEffect() const
{
    return false;
}

double NewProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NewProjectAudioProcessor::getNumPrograms()
{
    return programs_.size();
}

int NewProjectAudioProcessor::getCurrentProgram()
{
    return currentProgram_;
}

void NewProjectAudioProcessor::setCurrentProgram (int index)
{
    currentProgram_ = index;
}

const juce::String NewProjectAudioProcessor::getProgramName (int index)
{
    return juce::String(programs_.at(index));
}

void NewProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void NewProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

void NewProjectAudioProcessor::releaseResources()
{
}

bool NewProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return !(layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet());
}

void NewProjectAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    auto&& inputBuffer = sz::core::AudioBuffer<float>(buffer.getArrayOfWritePointers(),
                                                      buffer.getNumChannels(),
                                                      buffer.getNumSamples());

    engine_->process(inputBuffer);
}

bool NewProjectAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* NewProjectAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

void NewProjectAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

void NewProjectAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NewProjectAudioProcessor();
}

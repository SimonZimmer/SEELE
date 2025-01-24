#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "seeleCore/MemberParameterSet.h"

#include <functional>
#include <seeleCore/Factory.h>
#include <seeleCore/Config.h>

#include <core/AudioBuffer.h>

NewProjectAudioProcessor::NewProjectAudioProcessor()
: AudioProcessor (BusesProperties().withInput("Input",  juce::AudioChannelSet::stereo(), true)
    .withOutput("Output", juce::AudioChannelSet::stereo(), true))
, parameters_(*this, nullptr, "Parameters", createParameters())
, memberParameterSet_(std::make_unique<hidonash::MemberParameterSet>(parameters_))
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
    engine_ = hidonash::Factory().createEngine(*memberParameterSet_, sampleRate);
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
    auto&& inputBuffer = hidonash::core::AudioBuffer(buffer.getArrayOfWritePointers(),
                                                     static_cast<int>(buffer.getNumChannels()),
                                                     static_cast<int>(buffer.getNumSamples()));

    engine_->process(inputBuffer);

    buffer.copyFrom (0, 0, inputBuffer.getDataPointer(), inputBuffer.getNumSamples());
    buffer.copyFrom (1, 0, inputBuffer.getDataPointer(), inputBuffer.getNumSamples());
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

juce::AudioProcessorValueTreeState::ParameterLayout NewProjectAudioProcessor::createParameters()
{
    auto parameters = std::vector<std::unique_ptr<juce::RangedAudioParameter>>();

    for(auto n = 0; n < hidonash::config::constants::numMembers; ++n)
    {
        parameters.emplace_back(std::make_unique<juce::AudioParameterFloat>(hidonash::config::parameters::sanctityPrefix + std::to_string(n), "Seele " + std::to_string(n + 1) + " Sanctity",
                                                                            hidonash::config::parameters::minPitchFactor,
                                                                            hidonash::config::parameters::maxPitchFactor,
                                                                            hidonash::config::parameters::defaultPitchFactor));
        parameters.emplace_back(std::make_unique<juce::AudioParameterBool>(hidonash::config::parameters::summonStatePrefix + std::to_string(n), "Seele " + std::to_string(n + 1) + " Summoned",
                                                                           false));
    }

    return { parameters.begin(), parameters.end() };
}

juce::AudioProcessorValueTreeState& NewProjectAudioProcessor::getAudioProcessorValueTreeState()
{
    return parameters_;
}


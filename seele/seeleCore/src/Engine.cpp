#include "Engine.h"
#include "Config.h"
#include "IMemberParameterSet.h"
#include "PitchShifter.h"
#include "core/AudioBuffer.h"


namespace hidonash
{
    Engine::Engine(const IMemberParameterSet& memberParameterSet, double sampleRate,
                   FactoryPtr factory, size_t numMembers)
    : memberParameterSet_(memberParameterSet)
    , numMembers_(numMembers)
    {
        for(auto n = size_t{ 0 }; n < numMembers_; ++n)
        {
            pitchShifters_.emplace_back(factory->createPitchShifter(sampleRate, *factory));
            audioBuffers_.emplace_back(factory->createAudioBuffer(2, 4048));
        }
    }

    void Engine::process(core::IAudioBuffer& inputBuffer)
    {
        if (numMembers_ > 0 && memberParameterSet_.getSummonState(0))
        {
            audioBuffers_[0]->setSize(inputBuffer.getNumChannels(), inputBuffer.getNumSamples());
            audioBuffers_[0]->copyFrom(inputBuffer);

            pitchShifters_[0]->process(*audioBuffers_[0]);

            inputBuffer.copyFrom(*audioBuffers_[0]); // Replace the input with processed output
        }
    }
}


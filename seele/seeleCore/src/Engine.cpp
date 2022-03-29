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
        for(auto n = size_t{ 0 }; n < numMembers_; ++n)
        {
            pitchShifters_[n]->setPitchRatio(memberParameterSet_.getPitchRatio(n));
            audioBuffers_[n]->setSize(inputBuffer.getNumChannels(), inputBuffer.getNumSamples());
            audioBuffers_[n]->copyFrom(inputBuffer);
            pitchShifters_[n]->process(*audioBuffers_[n]);
        }

        inputBuffer.fill(0.f);

        for(auto n = size_t{ 0 }; n < numMembers_; ++n)
            inputBuffer.add(*audioBuffers_[n], inputBuffer.getNumSamples());

        inputBuffer.multiply(1.f / static_cast<float>(config::constants::numMembers), inputBuffer.getNumSamples());
    }
}


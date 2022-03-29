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
        auto numSummonedMembers = 0;
        for(auto n = size_t{ 0 }; n < numMembers_; ++n)
        {
            if(memberParameterSet_.getSummonState(n))
            {
                audioBuffers_[n]->setSize(inputBuffer.getNumChannels(), inputBuffer.getNumSamples());
                audioBuffers_[n]->copyFrom(inputBuffer);
                pitchShifters_[n]->setPitchRatio(memberParameterSet_.getSanctity(n));
                pitchShifters_[n]->process(*audioBuffers_[n]);
            }
            ++numSummonedMembers;
        }

        inputBuffer.fill(0.f);

        for(auto n = size_t{ 0 }; n < numMembers_; ++n)
            if(memberParameterSet_.getSummonState(n))
                inputBuffer.add(*audioBuffers_[n], inputBuffer.getNumSamples());

        inputBuffer.multiply(1.f / static_cast<float>(numSummonedMembers), inputBuffer.getNumSamples());
    }
}


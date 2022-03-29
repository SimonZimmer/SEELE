#include "Engine.h"
#include "Config.h"
#include "IMemberParameterSet.h"
#include "PitchShifter.h"
#include "core/AudioBuffer.h"


namespace hidonash
{
    Engine::Engine(const IMemberParameterSet& memberParameterSet, double sampleRate, FactoryPtr factory)
    : memberParameterSet_(memberParameterSet)
    {
        for(auto n = size_t{ 0 }; n < config::constants::numMembers; ++n)
        {
            pitchShifters_.emplace_back(std::make_unique<PitchShifter>(sampleRate, *factory));
            audioBuffers_.emplace_back(std::make_unique<core::AudioBuffer>(2, 4048));
        }
    }

    void Engine::process(core::IAudioBuffer& inputBuffer)
    {
        for(auto n = size_t{ 0 }; n < config::constants::numMembers; ++n)
        {
            pitchShifters_[n]->setPitchRatio(memberParameterSet_.getPitchRatio(n));
            audioBuffers_[n]->setSize(inputBuffer.getNumChannels(), inputBuffer.getNumSamples());
            audioBuffers_[n]->copyFrom(inputBuffer);
            pitchShifters_[n]->process(*audioBuffers_[n]);
        }

        inputBuffer.fill(0.f);

        for(auto n = size_t{ 0 }; n < config::constants::numMembers; ++n)
            inputBuffer.add(*audioBuffers_[n], inputBuffer.getNumSamples());

        inputBuffer.multiply(1 / config::constants::numMembers, inputBuffer.getNumSamples());
    }
}


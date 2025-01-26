#include "Engine.h"
#include "Config.h"
#include "IMemberParameterSet.h"
#include "core/AudioBuffer.h"


namespace hidonash
{
    Engine::Engine(const IMemberParameterSet& memberParameterSet, double sampleRate,
                   int samplesPerBlock, size_t numChannels, FactoryPtr factory)
    : memberParameterSet_(memberParameterSet)
    , internalBuffer_(numChannels, samplesPerBlock)
    {
        for(auto n = 0; n < config::constants::numMembers; ++n)
        {
            pitchShifterManagers_.emplace_back(factory->createPitchShifterManager(sampleRate, numChannels, *factory));
            audioBuffers_.emplace_back(factory->createAudioBuffer(numChannels, samplesPerBlock));
        }
    }

    void Engine::process(core::IAudioBuffer& inputBuffer)
    {
        internalBuffer_.fill(0.0f);

        int activeMembers = 0;
        for (auto n = 0; n < config::constants::numMembers; ++n)
            if (memberParameterSet_.getSummonState(n))
            {
                audioBuffers_[n]->copyFrom(inputBuffer);
                pitchShifterManagers_[n]->setPitchRatio(memberParameterSet_.getSanctity(n));
                pitchShifterManagers_[n]->process(*audioBuffers_[n]);
                internalBuffer_.add(*audioBuffers_[n], inputBuffer.getNumSamples());
                activeMembers++;
            }

        if (activeMembers <= 0)
            return;

        internalBuffer_.multiply(1.f / static_cast<float>(activeMembers), internalBuffer_.getNumSamples());

        inputBuffer.copyFrom(internalBuffer_);
    }
}


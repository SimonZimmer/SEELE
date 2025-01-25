#include "Engine.h"
#include "IMemberParameterSet.h"
#include "core/AudioBuffer.h"


namespace hidonash
{
    Engine::Engine(const IMemberParameterSet& memberParameterSet, double sampleRate, int samplesPerBlock,
                   FactoryPtr factory, size_t numMembers)
    : memberParameterSet_(memberParameterSet)
    , numMembers_(numMembers)
    {
        for(auto n = 0; n < numMembers_; ++n)
        {
            pitchShifterManagers_.emplace_back(factory->createPitchShifterManager(sampleRate, 2, *factory));
            audioBuffers_.emplace_back(factory->createAudioBuffer(2, samplesPerBlock));
        }
    }

    void Engine::process(core::IAudioBuffer& inputBuffer)
    {
        core::AudioBuffer outputBuffer(inputBuffer.getNumChannels(), inputBuffer.getNumSamples());
        outputBuffer.fill(0.f);

        int activeMembers = 0;
        for (auto n = 0; n < numMembers_; ++n)
        {
            if (memberParameterSet_.getSummonState(n))
            {
                audioBuffers_[n]->copyFrom(inputBuffer);
                pitchShifterManagers_[n]->setPitchRatio(memberParameterSet_.getSanctity(n));
                pitchShifterManagers_[n]->process(*audioBuffers_[n]);
                outputBuffer.add(*audioBuffers_[n], inputBuffer.getNumSamples());
                activeMembers++;
            }
        }

        if (activeMembers > 0)
            outputBuffer.multiply(1.f / static_cast<float>(activeMembers), inputBuffer.getNumSamples());
        else
            return;

        inputBuffer.copyFrom(outputBuffer);
    }
}


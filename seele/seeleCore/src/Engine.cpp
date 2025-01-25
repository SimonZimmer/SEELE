#include "Engine.h"
#include "IMemberParameterSet.h"
#include "core/AudioBuffer.h"


namespace hidonash
{
    Engine::Engine(const IMemberParameterSet& memberParameterSet, double sampleRate,
                   FactoryPtr factory, size_t numMembers)
    : memberParameterSet_(memberParameterSet)
    , numMembers_(numMembers)
    {
        for(auto n = 0; n < numMembers_; ++n)
        {
            pitchShifters_.emplace_back(factory->createPitchShifter(sampleRate, *factory));
            audioBuffers_.emplace_back(factory->createAudioBuffer(2, 4048));
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
                audioBuffers_[n]->setSize(inputBuffer.getNumChannels(), inputBuffer.getNumSamples());
                audioBuffers_[n]->copyFrom(inputBuffer);
                pitchShifters_[n]->setPitchRatio(memberParameterSet_.getSanctity(n));
                pitchShifters_[n]->process(*audioBuffers_[n]);
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


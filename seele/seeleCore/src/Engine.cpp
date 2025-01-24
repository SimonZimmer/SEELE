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
        int numSummonedMembers = 0;

        // Temporary buffer to accumulate results
        core::AudioBuffer outputBuffer(inputBuffer.getNumChannels(), inputBuffer.getNumSamples());
        outputBuffer.fill(0.f);

        for (auto n = size_t{0}; n < numMembers_; ++n)
        {
            if (memberParameterSet_.getSummonState(n))
            {
                // Prepare the audio buffer for processing
                audioBuffers_[n]->setSize(inputBuffer.getNumChannels(), inputBuffer.getNumSamples());
                audioBuffers_[n]->copyFrom(inputBuffer);

                // Set pitch shift ratio and process the buffer
                pitchShifters_[n]->setPitchRatio(memberParameterSet_.getSanctity(n));
                pitchShifters_[n]->process(*audioBuffers_[n]);

                // Accumulate the processed buffer into the output buffer
                outputBuffer.add(*audioBuffers_[n], inputBuffer.getNumSamples());

                ++numSummonedMembers;
            }
        }

        if (numSummonedMembers > 0)
        {
            // Normalize the accumulated output buffer
            outputBuffer.multiply(1.f / static_cast<float>(numSummonedMembers), inputBuffer.getNumSamples());
        }

        // Copy the output buffer back to the input buffer
        inputBuffer.copyFrom(outputBuffer);
    }
}


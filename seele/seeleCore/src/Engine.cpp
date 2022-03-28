#include "Engine.h"
#include "PitchShifter.h"
#include "core/AudioBuffer.h"


namespace hidonash
{
    Engine::Engine(std::atomic<float>& seele1Pitch, std::atomic<float>& seele2Pitch, double sampleRate, FactoryPtr factory)
    : seele1Pitch_(seele1Pitch)
    , seele2Pitch_(seele2Pitch)
    {
        for(auto n = size_t{ 0 }; n < 2; ++n)
        {
            pitchShifters_.emplace_back(std::make_unique<PitchShifter>(sampleRate, *factory));
            audioBuffers_.emplace_back(std::make_unique<core::AudioBuffer>(2, 4048));
        }
    }

    void Engine::process(core::IAudioBuffer& inputBuffer)
    {
        pitchShifters_[0]->setPitchRatio(seele1Pitch_);
        pitchShifters_[1]->setPitchRatio(seele2Pitch_);
        for(auto n = size_t{ 0 }; n < 2; ++n)
        {
            audioBuffers_[n]->setSize(inputBuffer.getNumChannels(), inputBuffer.getNumSamples());
            audioBuffers_[n]->copyFrom(inputBuffer);
            pitchShifters_[n]->process(*audioBuffers_[n]);
        }

        inputBuffer.fill(0.f);

        for(auto n = size_t{ 0 }; n < 2; ++n)
            inputBuffer.add(*audioBuffers_[n], inputBuffer.getNumSamples());

        inputBuffer.multiply(0.5f, inputBuffer.getNumSamples());
    }
}

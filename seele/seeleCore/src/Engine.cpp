#include "Engine.h"
#include "PitchShifter.h"
#include "core/AudioBuffer.h"


namespace hidonash
{
    Engine::Engine(std::atomic<float>& pitchRatio, double sampleRate, FactoryPtr factory)
    : pitchRatio_(pitchRatio)
    {
        for(auto n = size_t{ 0 }; n < 2; ++n)
        {
            pitchShifters_.emplace_back(std::make_unique<PitchShifter>(sampleRate, *factory));
            audioBuffers_.emplace_back(std::make_unique<core::AudioBuffer>(2, 4048));
        }
    }

    void Engine::process(core::IAudioBuffer& inputBuffer)
    {
        audioBuffers_[0]->setSize(inputBuffer.getNumChannels(), inputBuffer.getNumSamples());
        audioBuffers_[1]->setSize(inputBuffer.getNumChannels(), inputBuffer.getNumSamples());
        pitchShifters_[0]->setPitchRatio(0.5f);
        pitchShifters_[1]->setPitchRatio(pitchRatio_);
        audioBuffers_[0]->copyFrom(inputBuffer);
        audioBuffers_[1]->copyFrom(inputBuffer);
        pitchShifters_[0]->process(*audioBuffers_[0]);
        pitchShifters_[1]->process(*audioBuffers_[1]);
        inputBuffer.fill(0.f);
        inputBuffer.add(*audioBuffers_[0], inputBuffer.getNumSamples());
        inputBuffer.add(*audioBuffers_[1], inputBuffer.getNumSamples());
        inputBuffer.multiply(0.5f, inputBuffer.getNumSamples());
    }
}

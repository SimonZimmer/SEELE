#include "Engine.h"

namespace hidonash
{
    Engine::Engine(std::atomic<float>& pitchRatio, double sampleRate, FactoryPtr factory)
    : pitchShifter(sampleRate, std::move(factory))
    , pitchRatio_(pitchRatio)
    {
        pitchShifter.setPitchRatio(pitchRatio);
    }

    // TODO introduce prepareToPlay

    void Engine::process(core::AudioBuffer<float>& inputBuffer)
    {
        pitchShifter.setPitchRatio(pitchRatio_);
        pitchShifter.process(inputBuffer);
    }
}


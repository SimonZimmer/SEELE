#include "Engine.h"

namespace hidonash
{
    Engine::Engine(std::atomic<float>& pitchRatio, double sampleRate, FactoryPtr factory)
    : pitchShifter(sampleRate, std::move(factory))
    , pitchRatio_(pitchRatio)
    {
        pitchShifter.setPitchRatio(pitchRatio);
    }

    void Engine::process(core::IAudioBuffer& inputBuffer)
    {
        pitchShifter.setPitchRatio(pitchRatio_);
        pitchShifter.process(inputBuffer);
    }
}


#include "Engine.h"

namespace hidonash
{
    Engine::Engine(std::atomic<float>& pitchRatio, const IFactory& factory)
    : factory_(factory)
    , pitchRatio_(pitchRatio)
    {
        phaseVocoder_.setPitchRatio(pitchRatio);
    }

    void Engine::process(core::AudioBuffer<float>& inputBuffer)
    {
        phaseVocoder_.setPitchRatio(pitchRatio_);
        phaseVocoder_.process(inputBuffer);
    }
}

#include "Engine.h"

namespace sz
{
    Engine::Engine(std::atomic<float>& chopFrequency,
                   const IFactory& factory)
    : factory_(factory)
    , chopper_(factory_.createChopper(chopFrequency))
    {
    }

    void Engine::process(core::AudioBuffer<float>& inputBuffer)
    {
        chopper_->process(inputBuffer);
    }
}

#include "Engine.h"

namespace sz
{
    Engine::Engine(std::atomic<float>& parameterValue,
                   std::atomic<float>& chopGain,
                   const IFactory& factory)
    : factory_(factory)
    , chopper_(factory_.createChopper(parameterValue, chopGain))
    {
    }

    void Engine::process(core::AudioBuffer<float>& inputBuffer)
    {
        chopper_->process(inputBuffer);
    }
}

#include "Factory.h"
#include "Chopper.h"
#include "Engine.h"

namespace sz
{
    AudioProcessorPtr Factory::createChopper(std::atomic<float>& chopLength,
                                             std::atomic<float>& chopGain) const
    {
        return std::make_unique<Chopper>(chopLength, chopGain);
    }

    AudioProcessorPtr Factory::createEngine(std::atomic<float>& chopLength,
                                            std::atomic<float>& chopGain) const
    {
        return std::make_unique<Engine>(chopLength, chopGain);
    }
}

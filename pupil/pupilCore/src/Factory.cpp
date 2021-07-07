#include "Factory.h"
#include "Chopper.h"
#include "Engine.h"

namespace sz
{
    AudioProcessorPtr Factory::createChopper(std::atomic<float>& chopFrequency) const
    {
        return std::make_unique<Chopper>(chopFrequency);
    }

    AudioProcessorPtr Factory::createEngine(std::atomic<float>& chopFrequency) const
    {
        return std::make_unique<Engine>(chopFrequency);
    }
}

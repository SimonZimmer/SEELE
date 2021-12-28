#include "Factory.h"
#include "Engine.h"

namespace sz
{
    AudioProcessorPtr Factory::createEngine(std::atomic<float>& pitchRatio) const
    {
        return std::make_unique<Engine>(pitchRatio);
    }
}

#include "Factory.h"
#include "Engine.h"

namespace hidonash
{
    AudioProcessorPtr
    Factory::createEngine(std::atomic<float>& pitchRatio, double sampleRate) const
    {
        return std::make_unique<Engine>(pitchRatio, sampleRate);
    }
}

#include "Factory.h"
#include "AmplitudeModulation.h"
#include "Engine.h"

namespace sz
{
    AudioProcessorPtr Factory::createChopper(std::atomic<float>& chopFrequency) const
    {
        return std::make_unique<AmplitudeModulation>(chopFrequency);
    }

    AudioProcessorPtr Factory::createEngine(std::atomic<float>& chopFrequency,
                                            std::atomic<float>& pitchRatio) const
    {
        return std::make_unique<Engine>(chopFrequency, pitchRatio);
    }
}

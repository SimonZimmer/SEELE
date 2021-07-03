#include "Factory.h"

#include "DelayProcessor.h"

namespace sz
{
    AudioProcessorPtr Factory::createDelayProcessor(std::atomic<float>& parameterValue)
    {
        return std::make_unique<DelayProcessor>(parameterValue);
    }
}

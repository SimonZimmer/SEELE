#pragma once

#include <memory>

#include "IAudioProcessor.h"

namespace sz
{
    class IFactory
    {
    public:
        ~IFactory() = default;

        virtual AudioProcessorPtr createDelayProcessor(std::atomic<float>& parameterValue) = 0;
    };

    using FactoryPtr = std::unique_ptr<IFactory>;
}

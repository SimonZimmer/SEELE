#pragma once

#include <memory>

#include "IAudioProcessor.h"

namespace sz
{
    class IFactory
    {
    public:
        ~IFactory() = default;

        virtual AudioProcessorPtr createChopper(std::atomic<float>& chopFrequency) const = 0;

        virtual AudioProcessorPtr createEngine(std::atomic<float>& chopFrequency) const = 0;
    };

    using FactoryPtr = std::unique_ptr<IFactory>;
}

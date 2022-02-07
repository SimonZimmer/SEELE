#pragma once

#include <memory>

#include "IAudioProcessor.h"

namespace hidonash
{
    class IFactory
    {
    public:
        ~IFactory() = default;

        virtual AudioProcessorPtr
        createEngine(std::atomic<float>& pitchRatio, std::atomic<float>& fftFrameSize, double sampleRate) const = 0;
    };

    using FactoryPtr = std::unique_ptr<IFactory>;
}

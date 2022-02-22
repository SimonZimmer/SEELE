#pragma once

#include "IFactory.h"

namespace hidonash
{
    class Factory : public IFactory
    {
    public:
        ~Factory() = default;

        AudioProcessorPtr createEngine(std::atomic<float>& pitchRatio, double sampleRate) const override;
    };
}

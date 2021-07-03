#pragma once

#include "IFactory.h"

namespace sz
{
    class Factory : public IFactory
    {
    public:
        ~Factory() = default;

        AudioProcessorPtr createChopper(std::atomic<float>& chopLength,
                                        std::atomic<float>& chopGain) const override;

        AudioProcessorPtr createEngine(std::atomic<float>& chopLength,
                                       std::atomic<float>& chopGain) const override;
    };
}

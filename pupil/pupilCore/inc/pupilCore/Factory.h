#pragma once

#include "IFactory.h"

namespace sz
{
    class Factory : public IFactory
    {
    public:
        ~Factory() = default;

        AudioProcessorPtr createDelayProcessor(std::atomic<float>& parameterValue) override;
    };
}

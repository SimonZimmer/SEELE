#pragma once

#include "IFactory.h"

namespace sz
{
    class Factory : public IFactory
    {
    public:
        ~Factory() = default;

        AudioProcessorPtr createEngine(std::atomic<float>& pitchRatio) const override;
    };
}

#pragma once

#include "IFactory.h"

namespace hidonash
{
    class Factory : public IFactory
    {
    public:
        ~Factory() = default;

        AudioProcessorPtr createEngine(std::atomic<float>& pitchRatio, std::atomic<float>& fftFrameSize,
                                       double sampleRate) const override;
    };
}

#pragma once

#include <core/AudioBuffer.h>
#include <core/InterpolatingValue.h>

#include "IChannelProcessor.h"


namespace hidonash
{
    class DelayProcessor : public hidonash::IChannelProcessor
    {
    public:
        DelayProcessor(size_t maxDelaySamples, float delaySamples, double sampleRate);

        void process(core::IAudioBuffer::IChannel& input) override;

        void setDelayInSamples(int delayInSamples);

    private:
        std::vector<float> circularBuffer_;
        size_t writeIndex_;
        float readPosition_;
        float currentDelay_;
        float targetDelay_;
        double sampleRate_;
    };
}


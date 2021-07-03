#pragma once

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"

namespace sz
{
    class DelayProcessor : public IAudioProcessor
    {
    public:
        DelayProcessor(std::atomic<float>& parameterValue);
        ~DelayProcessor() = default;

        void process(core::AudioBuffer<float>& inputBuffer) override;

    private:
        std::atomic<float>& parameterValue_;
    };
}

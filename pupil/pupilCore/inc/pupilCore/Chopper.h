#pragma once

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"

namespace sz
{
    class Chopper : public IAudioProcessor
    {
    public:
        Chopper(std::atomic<float>& parameterValue, std::atomic<float>& chopGain);
        ~Chopper() = default;

        void process(core::AudioBuffer<float>& inputBuffer) override;

    private:
        std::atomic<float>& parameterValue_;
        std::atomic<float>& chopGain_;
    };
}

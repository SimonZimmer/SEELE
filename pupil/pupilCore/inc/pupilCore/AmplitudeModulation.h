#pragma once

#include <chrono>

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"

namespace sz
{
    class AmplitudeModulation : public IAudioProcessor
    {
    public:
        AmplitudeModulation(std::atomic<float>& chopFrequency);
        ~AmplitudeModulation() = default;

        void process(core::AudioBuffer<float>& inputBuffer) override;

    private:
        std::atomic<float>& chopFrequency_;
        float startValue_;
    };
}

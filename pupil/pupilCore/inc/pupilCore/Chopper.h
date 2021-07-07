#pragma once

#include <chrono>

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"

namespace sz
{
    class Chopper : public IAudioProcessor
    {
    public:
        Chopper(std::atomic<float>& chopFrequency);
        ~Chopper() = default;

        void process(core::AudioBuffer<float>& inputBuffer) override;

    private:
        std::atomic<float>& chopFrequency_;
        float startValue_;
    };
}

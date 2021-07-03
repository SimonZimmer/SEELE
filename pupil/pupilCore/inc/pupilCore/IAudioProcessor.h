#pragma once

#include <core/AudioBuffer.h>

namespace sz
{
    class IAudioProcessor
    {
    public:
        virtual ~IAudioProcessor() = default;

        virtual void process(core::AudioBuffer<float>& inputBuffer) = 0;
    };
}

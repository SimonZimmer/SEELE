#pragma once

#include <core/AudioBuffer.h>

namespace sz
{
    class IAudioProcessor
    {
    public:
        virtual process(AudioBuffer<float>& inputBuffer) = 0;
    };
}

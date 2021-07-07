#pragma once

#include <memory>

namespace sz
{
    class ISampleRamp
    {
    public:
        virtual ~ISampleRamp() = default;

        virtual float getNextGain() = 0;
    };

    using SampleRampPtr = std::unique_ptr<ISampleRamp>;
}

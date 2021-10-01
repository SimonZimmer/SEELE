#pragma once

#include "ISampleRamp.h"

namespace sz
{
    class SampleRamp : public ISampleRamp
    {
    public:
        SampleRamp(float startValue, float endValue, int fadeDuration);
        ~SampleRamp() override = default;

        float getNextGain() override;

    private:
        bool rampsUp_;
        float increment_;
        float gain_;
        const float gainDelta_;
        float fadeDuration_;
    };
}

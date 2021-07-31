#include "SampleRamp.h"

#include <cmath>

namespace sz
{
    SampleRamp::SampleRamp(float startValue, float endValue, int fadeDuration)
    : fadeDuration_(fadeDuration - 1)
    , gainDelta_(std::abs(startValue - endValue))
    {
        increment_ = (gainDelta_ / fadeDuration_);
        gain_ = startValue - increment_;

        if(startValue < endValue)
            rampsUp_ = true;
        else
            rampsUp_ = false;
    }

    float SampleRamp::getNextGain()
    {
        if (rampsUp_)
            gain_ = gain_ + increment_;
        else
            gain_ = gain_ - increment_;

        return gain_;
    }
}


#pragma once

#include <memory>

#include "IAudioProcessor.h"


namespace hidonash
{
    class IPitchShifter : public IAudioProcessor
    {
    public:
        virtual ~IPitchShifter() = default;

        virtual void setPitchRatio(float pitchRatio) = 0;
    };

    using PitchShifterPtr = std::unique_ptr<IPitchShifter>;
}


#pragma once

#include <memory>

#include "IAudioProcessor.h"


namespace hidonash
{
    class IPitchShifterManager : public IAudioProcessor
    {
    public:
        virtual ~IPitchShifterManager() = default;

        virtual void setPitchRatio(float pitchRatio) = 0;
    };

    using PitchShifterManagerPtr = std::unique_ptr<IPitchShifterManager>;
}


#pragma once

#include <memory>

#include "IChannelProcessor.h"


namespace hidonash
{
    class IPitchShifter : public IChannelProcessor
    {
    public:
        virtual ~IPitchShifter() = default;

        virtual void setPitchRatio(float pitchRatio) = 0;
    };

    using PitchShifterPtr = std::unique_ptr<IPitchShifter>;
}


#pragma once

#include <memory>

#include "IAudioProcessor.h"

namespace sz
{
    class IPhaseCorrector : public IAudioProcessor
    {
    public:
        ~IPhaseCorrector() override = default;

        virtual void setHopSize(size_t hopSize) = 0;
        virtual void resetPhases() = 0;
        virtual void setTimeStretchRatio(float ratio) = 0;
    };

    using PhaseCorrectorPtr = std::unique_ptr<IPhaseCorrector>;
}

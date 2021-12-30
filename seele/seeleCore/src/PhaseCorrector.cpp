#include "PhaseCorrector.h"

#include <cmath>
#include <algorithm>

namespace sz
{
    using namespace config;

    namespace
    {
        float principalArgument(float arg)
        {
            return static_cast<float>(std::fmod(arg + M_PI, -(2 * M_PI)) + M_PI);
        }
    }

    PhaseCorrector::PhaseCorrector()
    : synthPhaseIncrements_(window::length, 0)
    , previousFramePhases_(window::length, 0)
    {
        analysisHopSize_ = window::length / window::overlaps;
    }

    void PhaseCorrector::process(core::AudioBuffer<float>& buffer)
    {
        const auto bufferSize = buffer.getNumSamples();
        for (int i = 0, x = 0; i < bufferSize - 1; i += 2, ++x)
        {
            // Update phase increments for pitch shifting
            const auto real = buffer.getDataPointer()[i];
            const auto imag = buffer.getDataPointer()[i + 1];
            const auto mag = sqrtf(real * real + imag * imag);
            const auto phase = atan2(imag, real);
            const auto omega = 2 * M_PI * analysisHopSize_ * x / (float) window::length;

            const auto deltaPhase = omega + principalArgument(phase - previousFramePhases_[x] - omega);

            previousFramePhases_[x] = phase;
            synthPhaseIncrements_[x] = principalArgument(synthPhaseIncrements_[x] +
                                                        (deltaPhase * timeStretchRatio_));

            buffer.getDataPointer()[i] = mag * std::cos(synthPhaseIncrements_[x]);
            buffer.getDataPointer()[i + 1] = mag * std::sin(synthPhaseIncrements_[x]);
        }
    }

    void PhaseCorrector::setHopSize(size_t hopSize)
    {
        analysisHopSize_ = hopSize;
    }

    void PhaseCorrector::setTimeStretchRatio(float ratio)
    {
        timeStretchRatio_ = ratio;
    }

    void PhaseCorrector::resetPhases()
    {
        memset(previousFramePhases_.data(), 0, sizeof(float) * window::length);
        memset(synthPhaseIncrements_.data(), 0, sizeof(float) * window::length);
    }
}

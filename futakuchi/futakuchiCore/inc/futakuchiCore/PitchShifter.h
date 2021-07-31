#pragma once

#include <vector>
#include <algorithm>
#include <atomic>

#include <core/AudioBuffer.h>
#include "PhaseVocoder.h"

namespace sz
{
    template<typename FloatType = float>
    class PitchShifter : public PhaseVocoder<FloatType>
    {
    public:
        PitchShifter()
        : synthPhaseIncrements_(this->windowSize, 0)
        , previousFramePhases_(this->windowSize, 0)
        , pitchRatio_(1.f)
        {
        }

        void setPitchRatio(float ratio)
        {
            if (pitchRatio_ == ratio)
                return;

            const juce::SpinLock::ScopedLockType lock(this->paramLock);

            // Lower ratios require a larger amount of incoming samples
            // This will introduce more latency and large analysis and synthesis buffers
            pitchRatio_ = std::clamp(ratio, this->MinPitchRatio, this->MaxPitchRatio);

            this->synthesisHopSize = (int) (this->windowSize / (float) this->MinOverlapAmount);
            this->analysisHopSize = (int) round(this->synthesisHopSize / pitchRatio_);
            this->resampleSize = (int) std::ceil(this->windowSize * this->analysisHopSize / (float) this->synthesisHopSize);
            timeStretchRatio_ = this->synthesisHopSize / (float) this->analysisHopSize;

            // Rescaling due to OLA processing gain
            double accum = 0.0;

            for (int i = 0; i < this->windowSize; ++i)
                accum += this->windowBuffer.getDataPointer()[i] * (double) this->windowBuffer.getDataPointer()[i];

            accum /= this->synthesisHopSize;
            this->rescalingFactor = (float) accum;
            this->synthesisHopSize = this->analysisHopSize;

            // Reset phases
            memset(previousFramePhases_.data(), 0, sizeof(FloatType) * this->windowSize);
            memset(synthPhaseIncrements_.data(), 0, sizeof(FloatType) * this->windowSize);
        }

        void processImpl(const core::AudioBuffer<float>& buffer, const int bufferSize) override final
        {
            // Update phase increments for pitch shifting
            for (int i = 0, x = 0; i < bufferSize - 1; i += 2, ++x)
            {
                const auto&& bufferPtr = buffer.getDataPointer();
                const auto real = bufferPtr[i];
                const auto imag = bufferPtr[i + 1];
                const auto mag = sqrtf(real * real + imag * imag);
                const auto phase = atan2(imag, real);
                const auto omega = juce::MathConstants<float>::twoPi * this->analysisHopSize * x / (float) this->windowSize;

                const auto deltaPhase = omega + this->principalArgument(phase - previousFramePhases_[x] - omega);

                previousFramePhases_[x] = phase;
                synthPhaseIncrements_[x] = this->principalArgument(synthPhaseIncrements_[x] + (deltaPhase * timeStretchRatio_));

                bufferPtr[i] = mag * std::cos(synthPhaseIncrements_[x]);
                bufferPtr[i + 1] = mag * std::sin(synthPhaseIncrements_[x]);
            }
        }

    private:
        std::vector<FloatType> synthPhaseIncrements_;
        std::vector<FloatType> previousFramePhases_;

        std::atomic<float> pitchRatio_;
        float timeStretchRatio_ = 1.f;
    };
}

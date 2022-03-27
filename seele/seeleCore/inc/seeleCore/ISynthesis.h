#pragma once

#include <memory>
#include <array>
#include <juce_dsp/juce_dsp.h>

#include "Config.h"

namespace hidonash
{
    class ISynthesis
    {
    public:
        virtual ~ISynthesis() = default;

        virtual std::array<float, 8192> getMagnitudeBuffer() = 0;

        virtual std::array<float, 8192> getFrequencyBuffer() = 0;

        virtual void reset() = 0;

        virtual void perform(juce::dsp::Complex<float>* fftWorkspace) = 0;
    };

    using SynthesisPtr = std::unique_ptr<ISynthesis>;
}

#pragma once

#include <memory>
#include <vector>
//TODO get rid of juce
#include <juce_dsp/juce_dsp.h>

#include "Config.h"

namespace hidonash
{
    class ISynthesis
    {
    public:
        virtual ~ISynthesis() = default;

        virtual std::array<float, config::constants::analysisSize>& getMagnitudeBuffer() = 0;

        virtual std::array<float, config::constants::analysisSize>& getFrequencyBuffer() = 0;

        virtual void reset() = 0;

        virtual void perform(juce::dsp::Complex<float>* fftWorkspace) = 0;
    };

    using SynthesisPtr = std::unique_ptr<ISynthesis>;
}

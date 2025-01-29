#pragma once

#include <array>
#include <memory>
//TODO get rid of juce
#include <juce_dsp/juce_dsp.h>

#include "Config.h"

namespace hidonash
{
    class IAnalysis
    {
    public:
        virtual ~IAnalysis() = default;

        virtual std::array<float, config::constants::analysisSize> getMagnitudeBuffer() const = 0;

        virtual std::array<float, config::constants::analysisSize> getFrequencyBuffer() const = 0;

        virtual void perform(juce::dsp::Complex<float>* fftWorkspace) = 0;
    };

    using AnalysisPtr = std::unique_ptr<IAnalysis>;
}

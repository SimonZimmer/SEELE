#pragma once

#include <memory>
#include <array>
//TODO get rid of juce
#include <juce_dsp/juce_dsp.h>

namespace hidonash
{
    class IAnalysis
    {
    public:
        virtual ~IAnalysis() = default;

        virtual std::array<float, 8192> getMagnitudeBuffer() const = 0;

        virtual std::array<float, 8192> getFrequencyBuffer() const = 0;

        virtual void perform(juce::dsp::Complex<float>* fftWorkspace) = 0;
    };

    using AnalysisPtr = std::unique_ptr<IAnalysis>;
}

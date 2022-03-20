#pragma once

#include <memory>

namespace hidonash
{
    class IAnalysis
    {
    public:
        virtual ~IAnalysis() = default;

        virtual std::array<float, 8192> getMagnitudeBuffer() = 0;

        virtual std::array<float, 8192> getFrequencyBuffer() = 0;

        virtual void perform(juce::dsp::Complex<float>* fftWorkspace) = 0;
    };

    using AnalysisPtr = std::unique_ptr<IAnalysis>;
}

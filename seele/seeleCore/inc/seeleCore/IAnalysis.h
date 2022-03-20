#pragma once

#include <memory>
#include <vector>
//TODO get rid of juce
#include <juce_dsp/juce_dsp.h>

namespace hidonash
{
    class IAnalysis
    {
    public:
        virtual ~IAnalysis() = default;

        virtual const std::vector<float>& getMagnitudeBuffer() = 0;

        virtual const std::vector<float>& getFrequencyBuffer() = 0;

        virtual void perform(juce::dsp::Complex<float>* fftWorkspace) = 0;
    };

    using AnalysisPtr = std::unique_ptr<IAnalysis>;
}

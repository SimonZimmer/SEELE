#pragma once

#include <array>

#include <juce_dsp/juce_dsp.h>

#include <core/AudioBuffer.h>

#include "IAnalysis.h"

namespace hidonash
{
    class Analysis : public IAnalysis
    {
    public:
        explicit Analysis(int freqPerBin);
        ~Analysis() override = default;

        std::array<float, 8192> getMagnitudeBuffer() override;

        std::array<float, 8192> getFrequencyBuffer() override;

        void perform(juce::dsp::Complex<float>* fftWorkspace) override;

    private:
        int freqPerBin_;

        std::array<float, 8192/2+1> lastPhase_;
        std::array<float, 8192> analysisFrequencyBuffer_;
        std::array<float, 8192> analysisMagnitudeBuffer_;
    };

}

#pragma once

#include <array>

#include <juce_dsp/juce_dsp.h>

#include <core/AudioBuffer.h>

#include "IAnalysis.h"
#include "Config.h"


namespace hidonash
{
    class Analysis : public IAnalysis
    {
    public:
        explicit Analysis(int freqPerBin);
        ~Analysis() override = default;

        std::array<float, config::constants::analysisSize> getMagnitudeBuffer() const override;

        std::array<float, config::constants::analysisSize> getFrequencyBuffer() const override;

        void perform(juce::dsp::Complex<float>* fftWorkspace) override;

    private:
        int freqPerBin_;

        std::array<float, config::constants::analysisSize / 2 + 1> lastPhase_;
        std::array<float, config::constants::analysisSize> analysisFrequencyBuffer_;
        std::array<float, config::constants::analysisSize> analysisMagnitudeBuffer_;
    };

}


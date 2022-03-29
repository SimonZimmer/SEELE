#pragma once

#include "IAnalysis.h"


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


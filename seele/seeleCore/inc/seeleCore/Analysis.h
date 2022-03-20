#pragma once

#include "IAnalysis.h"


namespace hidonash
{
    class Analysis : public IAnalysis
    {
    public:
        explicit Analysis(int freqPerBin);
        ~Analysis() override = default;

        const std::vector<float>& getMagnitudeBuffer() override;

        const std::vector<float>& getFrequencyBuffer() override;

        void perform(juce::dsp::Complex<float>* fftWorkspace) override;

    private:
        int freqPerBin_;

        std::vector<float> lastPhase_;
        std::vector<float> analysisFrequencyBuffer_;
        std::vector<float> analysisMagnitudeBuffer_;
    };

}

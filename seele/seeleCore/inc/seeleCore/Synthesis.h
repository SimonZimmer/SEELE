#pragma once

#include "ISynthesis.h"
#include "Config.h"


namespace hidonash
{
    class Synthesis : public ISynthesis
    {
    public:
        explicit Synthesis(int freqPerBin);
        ~Synthesis() override = default;

        void perform(juce::dsp::Complex<float>* fftWorkspace, const IAnalysis& analysis, float pitchFactor) override;

    private:
        int freqPerBin_;

        std::array<float, 8192 / 2 + 1> sumPhase_;
        std::array<float, 8192> frequencyBuffer_;
        std::array<float, 8192> magnitudeBuffer_;
    };

}

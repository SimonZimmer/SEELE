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

        std::array<float, config::constants::analysisSize>& getMagnitudeBuffer() override;

        std::array<float, config::constants::analysisSize>& getFrequencyBuffer() override;

        void perform(juce::dsp::Complex<float>* fftWorkspace) override;

    private:
        int freqPerBin_;

        std::array<float, config::constants::analysisSize / 2 + 1> sumPhase_;
        std::array<float, config::constants::analysisSize> frequencyBuffer_;
        std::array<float, config::constants::analysisSize> magnitudeBuffer_;
    };

}

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

        std::array<float, 8192> getMagnitudeBuffer() override;

        std::array<float, 8192> getFrequencyBuffer() override;

        void reset() override;

        void perform(juce::dsp::Complex<float>* fftWorkspace) override;

    private:
        int freqPerBin_;

        std::array<float, 8192 / 2 + 1> sumPhase_;
        std::array<float, 8192> frequencyBuffer_;
        std::array<float, 8192> magnitudeBuffer_;
    };

}

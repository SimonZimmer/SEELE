#include <cmath>

#include "Synthesis.h"
#include "Config.h"


namespace hidonash
{
    Synthesis::Synthesis(int freqPerBin)
    : freqPerBin_(freqPerBin)
    {
        std::fill(magnitudeBuffer_.begin(), magnitudeBuffer_.end(), 0.f);
        std::fill(frequencyBuffer_.begin(), frequencyBuffer_.end(), 0.f);
    }

    //TODO feature envy?
    std::array<float, config::constants::analysisSize>& Synthesis::getMagnitudeBuffer()
    {
        return magnitudeBuffer_;
    }

    std::array<float, config::constants::analysisSize>& Synthesis::getFrequencyBuffer()
    {
        return frequencyBuffer_;
    }

    void Synthesis::perform(juce::dsp::Complex<float>* fftWorkspace)
    {
        for (auto k = 0; k <= config::constants::fftFrameSize; k++)
        {
            const auto magnitude = magnitudeBuffer_[k];
            auto phase = frequencyBuffer_[k];
            /* subtract bin mid frequency */
            auto phaseDifference = phase - (double)k * freqPerBin_;
            /* get bin deviation from freq deviation */
            phaseDifference /= freqPerBin_;
            /* take osamp into account */
            phaseDifference = 2. * M_PI * phaseDifference / config::constants::oversamplingFactor;
            /* add the overlap phase advance back in */
            phaseDifference += (double)k * config::constants::expectedPhaseDifference;
            /* accumulate delta phase to get bin phase */
            sumPhase_[k] += phaseDifference;
            phase = sumPhase_[k];
            fftWorkspace[k].real(magnitude * cos(phase));
            fftWorkspace[k].imag(magnitude * sin(phase));
        }
    }
}

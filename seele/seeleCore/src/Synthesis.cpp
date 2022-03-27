#include <cmath>

#include "Synthesis.h"
#include "Config.h"


namespace hidonash
{
    Synthesis::Synthesis(int freqPerBin)
    : freqPerBin_(freqPerBin)
    {
    }

    void Synthesis::perform(juce::dsp::Complex<float>* fftWorkspace, const IAnalysis& analysis, float pitchFactor)
    {
        memset(magnitudeBuffer_.data(), 0, config::constants::fftFrameSize * sizeof(float));
        memset(frequencyBuffer_.data(), 0, config::constants::fftFrameSize * sizeof(float));

        auto&& analysisMagnitudeBuffer = analysis.getMagnitudeBuffer();
        auto&& analysisFrequencyBuffer = analysis.getFrequencyBuffer();
        for (auto k = 0; k <= config::constants::fftFrameSize / 2; k++)
        {
            auto index = k * pitchFactor;
            if (index <= (config::constants::fftFrameSize / 2))
            {
                magnitudeBuffer_[index] += analysisMagnitudeBuffer[k];
                frequencyBuffer_[index] = analysisFrequencyBuffer[k] * pitchFactor;
            }
        }

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
            /* get real and imag part and re-interleave */
            fftWorkspace[k].real(magnitude * cos(phase));
            fftWorkspace[k].imag(magnitude * sin(phase));
        }
    }
}

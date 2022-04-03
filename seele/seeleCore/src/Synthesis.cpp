#include <cmath>

#include "Synthesis.h"
#include "Config.h"


namespace hidonash
{
    Synthesis::Synthesis(int freqPerBin, AnalysisPtr analysis)
    : analysis_(std::move(analysis))
    , freqPerBin_(freqPerBin)
    {
    }

    void Synthesis::perform(juce::dsp::Complex<float>* fftWorkspace, float pitchFactor)
    {
        analysis_->perform(fftWorkspace);
        memset(magnitudeBuffer_.data(), 0, config::constants::fftFrameSize * sizeof(float));
        memset(frequencyBuffer_.data(), 0, config::constants::fftFrameSize * sizeof(float));

        auto&& analysisMagnitudeBuffer = analysis_->getMagnitudeBuffer();
        auto&& analysisFrequencyBuffer = analysis_->getFrequencyBuffer();
        for (auto sa = 0; sa <= config::constants::fftFrameSize / 2; sa++)
        {
            auto index = sa * pitchFactor;
            if (index <= (config::constants::fftFrameSize / 2))
            {
                magnitudeBuffer_[index] += analysisMagnitudeBuffer[sa];
                frequencyBuffer_[index] = analysisFrequencyBuffer[sa] * pitchFactor;
            }
        }

        for (auto sa = 0; sa <= config::constants::fftFrameSize; sa++)
        {
            const auto magnitude = magnitudeBuffer_[sa];
            auto phase = frequencyBuffer_[sa];
            /* subtract bin mid frequency */
            auto phaseDifference = phase - static_cast<double>(sa) * freqPerBin_;
            /* get bin deviation from freq deviation */
            phaseDifference /= freqPerBin_;
            phaseDifference = 2. * config::constants::pi * phaseDifference / config::constants::oversamplingFactor;
            /* add the overlap phase advance back in */
            phaseDifference += static_cast<double>(sa) * config::constants::expectedPhaseDifference;
            /* accumulate delta phase to get bin phase */
            sumPhase_[sa] += phaseDifference;
            phase = sumPhase_[sa];
            fftWorkspace[sa].real(magnitude * cos(phase));
            fftWorkspace[sa].imag(magnitude * sin(phase));
        }
    }
}


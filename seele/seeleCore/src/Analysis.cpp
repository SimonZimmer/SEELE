#include <cmath>

#include "Analysis.h"
#include "Config.h"


namespace hidonash
{
    Analysis::Analysis(int freqPerBin)
    : freqPerBin_(freqPerBin)
    {
        lastPhase_.resize(config::constants::analysisSize / 2 + 1);
        analysisMagnitudeBuffer_.resize(config::constants::analysisSize);
        analysisFrequencyBuffer_.resize(config::constants::analysisSize);

        std::fill(analysisMagnitudeBuffer_.begin(), analysisMagnitudeBuffer_.end(), 0.f);
        std::fill(analysisFrequencyBuffer_.begin(), analysisMagnitudeBuffer_.end(), 0.f);
    }

    const std::vector<float>& Analysis::getMagnitudeBuffer()
    {
        return analysisMagnitudeBuffer_;
    }

    const std::vector<float>& Analysis::getFrequencyBuffer()
    {
        return analysisFrequencyBuffer_;
    }

    void Analysis::perform(juce::dsp::Complex<float>* fftWorkspace)
    {
        for (auto sa = 0; sa <= config::constants::fftFrameSize / 2; sa++)
        {
            const auto real = fftWorkspace[sa].real();
            const auto imag = fftWorkspace[sa].imag();
            /* compute magnitude and phase */
            const auto magnitude = 2. * sqrt(real * real + imag * imag);
            const auto phase = atan2(imag,real);
            /* compute phase difference */
            auto phaseDifference = phase - lastPhase_[sa];
            lastPhase_[sa] = phase;
            /* subtract expected phase difference */
            phaseDifference -= sa * config::constants::expectedPhaseDifference;
            /* map delta phase into +/- Pi interval */
            long qpd = phaseDifference / M_PI;
            if (qpd >= 0) qpd += qpd&1;
            else qpd -= qpd&1;
            phaseDifference -= M_PI * (double)qpd;
            /* get deviation from bin frequency from the +/- Pi interval */
            phaseDifference = static_cast<float>(config::constants::oversamplingFactor) * phaseDifference / (2. * M_PI);
            /* compute the sa-th partials' true frequency */
            phaseDifference = static_cast<float>(sa) * freqPerBin_ + phaseDifference * static_cast<float>(freqPerBin_);

            analysisMagnitudeBuffer_[sa] = static_cast<float>(magnitude);
            analysisFrequencyBuffer_[sa] = phaseDifference;
        }
    }
}

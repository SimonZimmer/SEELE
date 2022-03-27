#include <cmath>

#include "Analysis.h"


namespace hidonash
{
    Analysis::Analysis(int freqPerBin)
    : freqPerBin_(freqPerBin)
    {
    }

    std::array<float, config::constants::analysisSize> Analysis::getMagnitudeBuffer() const
    {
        return analysisMagnitudeBuffer_;
    }

    std::array<float, config::constants::analysisSize> Analysis::getFrequencyBuffer() const
    {
        return analysisFrequencyBuffer_;
    }

    void Analysis::perform(juce::dsp::Complex<float>* fftWorkspace)
    {
        for (auto k = 0; k <= config::constants::fftFrameSize/ 2; k++)
        {
            const auto real = fftWorkspace[k].real();
            const auto imag = fftWorkspace[k].imag();
            /* compute magnitude and phase */
            const auto magnitude = 2. * sqrt(real * real + imag * imag);
            const auto phase = atan2(imag,real);
            /* compute phase difference */
            auto phaseDifference = phase - lastPhase_[k];
            lastPhase_[k] = phase;
            /* subtract expected phase difference */
            phaseDifference -= (double)k * config::constants::expectedPhaseDifference;
            /* map delta phase into +/- Pi interval */
            long qpd = phaseDifference / M_PI;
            if (qpd >= 0) qpd += qpd&1;
            else qpd -= qpd&1;
            phaseDifference -= M_PI * (double)qpd;
            /* get deviation from bin frequency from the +/- Pi interval */
            phaseDifference = config::constants::oversamplingFactor * phaseDifference / (2. * M_PI);
            /* compute the k-th partials' true frequency */
            phaseDifference = (double)k * freqPerBin_ + phaseDifference * freqPerBin_;

            analysisMagnitudeBuffer_[k] = magnitude;
            analysisFrequencyBuffer_[k] = phaseDifference;
        }
    }
}

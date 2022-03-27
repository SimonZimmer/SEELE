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
        for (auto sa = 0; sa <= config::constants::fftFrameSize/ 2; sa++)
        {
            const auto real = fftWorkspace[sa].real();
            const auto imag = fftWorkspace[sa].imag();
            const auto magnitude = 2. * sqrt(real * real + imag * imag);
            const auto phase = atan2(imag,real);
            auto phaseDifference = phase - lastPhase_[sa];
            lastPhase_[sa] = phase;
            phaseDifference -= static_cast<double>(sa) * config::constants::expectedPhaseDifference;
            /* map delta phase into +/- Pi interval */
            long qpd = phaseDifference / M_PI;
            if (qpd >= 0)
                qpd += qpd&1;
            else
                qpd -= qpd&1;
            phaseDifference -= M_PI * static_cast<double>(qpd);
            /* get deviation from bin frequency from the +/- Pi interval */
            phaseDifference = config::constants::oversamplingFactor * phaseDifference / (2. * M_PI);
            /* compute the k-th partials' true frequency */
            phaseDifference = static_cast<double>(sa) * freqPerBin_ + phaseDifference * freqPerBin_;

            analysisMagnitudeBuffer_[sa] = magnitude;
            analysisFrequencyBuffer_[sa] = phaseDifference;
        }
    }
}


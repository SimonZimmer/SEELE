#include <cmath>

#include "PitchShifter.h"
#include "Config.h"


namespace hidonash
{
    namespace
    {
        double getWindowFactor(size_t k, size_t windowSize)
        {
            return (-.5 * cos(2. * M_PI * (double)k / (double)windowSize) + .5);
        }
    }

    PitchShifter::PitchShifter(double sampleRate)
    : sampleRate_(sampleRate)
    {
        fftFrameSize_ = config::parameters::fftFrameSize;
        const auto fftOrder = std::log2(fftFrameSize_);
        fft_ = std::make_unique<juce::dsp::FFT>(static_cast<int>(fftOrder));
        gainCompensation_ = std::pow(10, (65. / 20.));
    }

    void PitchShifter::process(core::AudioBuffer<float>& audioBuffer)
    {
        auto stepSize = fftFrameSize_ / config::constants::oversamplingFactor;
        auto freqPerBin = sampleRate_ / (double)fftFrameSize_;
        auto expectedPhaseDifference = 2. * M_PI * (double)stepSize / (double)fftFrameSize_;
        auto inFifoLatency = fftFrameSize_ - stepSize;
        static long sampleCounter = false;
        if (sampleCounter == false) sampleCounter = inFifoLatency;

        //TODO sum stereo to mono instead of processing left channel only
        for(auto sa = 0; sa < audioBuffer.getNumSamples(); ++sa)
            audioBuffer[0][sa] = audioBuffer[0][sa] + audioBuffer[1][sa] * 0.5f;

        auto indata = audioBuffer.getDataPointer();
        auto outdata = audioBuffer.getDataPointer();
        for (auto sa = 0; sa < audioBuffer.getNumSamples(); sa++)
        {
            /* As long as we have not yet collected enough data just read in */
            fifoIn_[sampleCounter] = indata[sa];
            outdata[sa] = fifoOut_[sampleCounter - inFifoLatency];
            sampleCounter++;

            /* now we have enough data for processing */
            if (sampleCounter >= fftFrameSize_)
            {
                sampleCounter = inFifoLatency;

                /* do windowing */
                for (auto k = 0; k < fftFrameSize_;k++)
                {
                    fftWorkspace_[k].real(fifoIn_[k] * getWindowFactor(k, fftFrameSize_));
                    fftWorkspace_[k].imag(0.);
                }

                fft(fftWorkspace_.data(), false);
                analysis(freqPerBin, expectedPhaseDifference);

                /* this does the actual pitch shifting */
                memset(synthesisMagnitudeBuffer_.data(), 0, fftFrameSize_ * sizeof(float));
                memset(synthesisFrequencyBuffer_.data(), 0, fftFrameSize_ * sizeof(float));
                for (auto k = 0; k <= fftFrameSize_ / 2; k++)
                {
                    auto index = k * pitchFactor_;
                    if (index <= (fftFrameSize_ / 2))
                    {
                        synthesisMagnitudeBuffer_[index] += analysisMagnitudeBuffer_[k];
                        synthesisFrequencyBuffer_[index] = analysisFrequencyBuffer_[k] * pitchFactor_;
                    }
                }

                synthesis(freqPerBin, expectedPhaseDifference);
                fft(fftWorkspace_.data(), true);

                /* do windowing and add to output accumulator */
                for(auto k = 0; k < fftFrameSize_; k++)
                    outputAccumulationBuffer_[k] += 2. * getWindowFactor(k, fftFrameSize_) * fftWorkspace_[k].real() / ((fftFrameSize_ / 2) * config::constants::oversamplingFactor);

                for (auto k = 0; k < stepSize; k++) fifoOut_[k] = outputAccumulationBuffer_[k];

                /* shift accumulator */
                memmove(outputAccumulationBuffer_.data(), outputAccumulationBuffer_.data() + stepSize, fftFrameSize_ * sizeof(float));
                /* move input FIFO */
                for (auto k = 0; k < inFifoLatency; k++) fifoIn_[k] = fifoIn_[k + stepSize];
            }
        }

        for(auto ch = 0; ch < audioBuffer.getNumChannels(); ++ch)
            for(auto sa = 0; sa < audioBuffer.getNumSamples(); ++sa)
                audioBuffer[ch][sa] = audioBuffer[ch][sa] * gainCompensation_;
    }

    void PitchShifter::synthesis(int freqPerBin, double expectedPhaseDifference)
    {
        for (auto k = 0; k <= fftFrameSize_; k++)
        {
            const auto magnitude = synthesisMagnitudeBuffer_[k];
            auto phase = synthesisFrequencyBuffer_[k];
            /* subtract bin mid frequency */
            auto phaseDifference = phase - (double)k * freqPerBin;
            /* get bin deviation from freq deviation */
            phaseDifference /= freqPerBin;
            /* take osamp into account */
            phaseDifference = 2. * M_PI * phaseDifference / config::constants::oversamplingFactor;
            /* add the overlap phase advance back in */
            phaseDifference += (double)k * expectedPhaseDifference;
            /* accumulate delta phase to get bin phase */
            sumPhase_[k] += phaseDifference;
            phase = sumPhase_[k];
            /* get real and imag part and re-interleave */
            fftWorkspace_[k].real(magnitude * cos(phase));
            fftWorkspace_[k].imag(magnitude * sin(phase));
        }
    }

    void PitchShifter::analysis(int freqPerBin, double expectedPhaseDifference)
    {
        for (auto k = 0; k <= fftFrameSize_ / 2; k++)
        {
            const auto real = fftWorkspace_[k].real();
            const auto imag = fftWorkspace_[k].imag();
            /* compute magnitude and phase */
            const auto magnitude = 2. * sqrt(real * real + imag * imag);
            const auto phase = atan2(imag,real);
            /* compute phase difference */
            auto phaseDifference = phase - lastPhase_[k];
            lastPhase_[k] = phase;
            /* subtract expected phase difference */
            phaseDifference -= (double)k * expectedPhaseDifference;
            /* map delta phase into +/- Pi interval */
            long qpd = phaseDifference / M_PI;
            if (qpd >= 0) qpd += qpd&1;
            else qpd -= qpd&1;
            phaseDifference -= M_PI * (double)qpd;
            /* get deviation from bin frequency from the +/- Pi interval */
            phaseDifference = config::constants::oversamplingFactor * phaseDifference / (2. * M_PI);
            /* compute the k-th partials' true frequency */
            phaseDifference = (double)k * freqPerBin + phaseDifference * freqPerBin;

            analysisMagnitudeBuffer_[k] = magnitude;
            analysisFrequencyBuffer_[k] = phaseDifference;
        }
    }

    void PitchShifter::fft(juce::dsp::Complex<float>* fftBuffer, bool inverse)
    {
        fft_->perform(fftBuffer, fftBuffer, inverse);
    }

    void PitchShifter::setPitchRatio(float pitchRatio)
    {
        pitchFactor_ = pitchRatio;
    }
}

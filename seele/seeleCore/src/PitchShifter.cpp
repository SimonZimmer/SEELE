#include <cmath>

#include "PitchShifter.h"
#include "Config.h"


namespace hidonash
{
    PitchShifter::PitchShifter(double sampleRate)
    : sampleRate_(sampleRate)
    {
        std::memset(inFifo_.data(), 0, max_frame_length_ * sizeof(float));
        std::memset(outFifo_.data(), 0, max_frame_length_ * sizeof(float));
        std::memset(fftWorkspace_.data(), 0, 2 * max_frame_length_ * sizeof(float));
        std::memset(lastPhase_.data(), 0, (max_frame_length_ / 2 + 1) * sizeof(float));
        std::memset(sumPhase_.data(), 0, (max_frame_length_ / 2 + 1) * sizeof(float));
        std::memset(outputAccumulator_.data(), 0, 2 * max_frame_length_ * sizeof(float));
        std::memset(gAnaFreq.data(), 0, max_frame_length_*sizeof(float));
        std::memset(gAnaMagn.data(), 0, max_frame_length_*sizeof(float));
    }

    void PitchShifter::process(core::AudioBuffer<float>& audioBuffer)
    {
        static long numAccumulatedSamples = false;
        double magnitude, phase, phase_difference, real, imag;
        double frequencyResolution, expectedPhaseDifference;
        long i, k, qpd, index, inFifoLatency, stepSize;

        /* set up some handy variables */
        stepSize = fftFrameSize_ / config::constants::oversamplingFactor;
        frequencyResolution = sampleRate_ / (double)fftFrameSize_;
        expectedPhaseDifference = 2. * M_PI * (double)stepSize / (double)fftFrameSize_;
        inFifoLatency = fftFrameSize_ - stepSize;
        if (numAccumulatedSamples == false) numAccumulatedSamples = inFifoLatency;

        /* main processing loop */
        auto indata = audioBuffer.getDataPointer();
        auto outdata = audioBuffer.getDataPointer();
        for (i = 0; i < audioBuffer.getNumSamples(); i++)
        {
            /* As long as we have not yet collected enough data just read in */
            inFifo_[numAccumulatedSamples] = indata[i];
            outdata[i] = outFifo_[numAccumulatedSamples - inFifoLatency];
            numAccumulatedSamples++;

            /* now we have enough data for processing */
            if (numAccumulatedSamples >= fftFrameSize_)
            {
                numAccumulatedSamples = inFifoLatency;

                /* do windowing and re,im interleave */
                for (k = 0; k < fftFrameSize_; k++)
                {
                    fftWorkspace_[2 * k] = inFifo_[k] * window_[k];
                    fftWorkspace_[2 * k + 1] = 0.;
                }

                /* ***************** ANALYSIS ******************* */
                fft(fftWorkspace_.data(), fftFrameSize_, false);

                /* this is the analysis step */
                for (k = 0; k <= fftFrameSize_ / 2; k++)
                {
                    /* de-interlace FFT buffer */
                    real = fftWorkspace_[2 * k];
                    imag = fftWorkspace_[2 * k + 1];

                    /* compute magnitude and phase */
                    magnitude = 2. * sqrt(real * real + imag * imag);
                    phase = atan2(imag, real);

                    /* compute phase difference */
                    phase_difference = phase - lastPhase_[k];
                    lastPhase_[k] = phase;
                    /* subtract expected phase difference */
                    phase_difference -= (double)k * expectedPhaseDifference;
                    /* map delta phase into +/- Pi interval */
                    qpd = phase_difference / M_PI;
                    if (qpd >= 0) qpd += qpd&1;
                    else qpd -= qpd&1;
                    phase_difference -= M_PI * (double)qpd;
                    /* get deviation from bin frequency from the +/- Pi interval */
                    phase_difference = config::constants::oversamplingFactor * phase_difference / (2. * M_PI);
                    /* compute the k-th partials' true frequency */
                    phase_difference = (double)k * frequencyResolution + phase_difference * frequencyResolution;

                    /* store magnitude and true frequency in analysis arrays */
                    gAnaMagn[k] = magnitude;
                    gAnaFreq[k] = phase_difference;
                }

                /* ***************** PROCESSING ******************* */
                /* this does the actual pitch shifting */
                memset(synthesisMagnitude_.data(), 0, fftFrameSize_ * sizeof(float));
                memset(synthesisFrequencyBuffer_.data(), 0, fftFrameSize_ * sizeof(float));
                for (k = 0; k <= (fftFrameSize_ / 2); k++)
                {
                    index = k * pitchFactor_;
                    if (index <= (fftFrameSize_ / 2))
                    {
                        synthesisMagnitude_[index] += gAnaMagn[k];
                        synthesisFrequencyBuffer_[index] = gAnaFreq[k] * pitchFactor_;
                    }
                }

                /* ***************** SYNTHESIS ******************* */
                /* this is the synthesis step */
                for (k = 0; k <= (fftFrameSize_ / 2); k++)
                {
                    /* get magnitude and true frequency from synthesis arrays */
                    magnitude = synthesisMagnitude_[k];

                    phase_difference = synthesisFrequencyBuffer_[k];
                    /* subtract bin mid frequency */
                    phase_difference -= static_cast<double>(k) * frequencyResolution;
                    /* get bin deviation from freq deviation */
                    phase_difference /= frequencyResolution;
                    /* take osamp into account */
                    phase_difference = 2. * M_PI * phase_difference / config::constants::oversamplingFactor;
                    /* add the overlap phase advance back in */
                    phase_difference += static_cast<double>(k) * expectedPhaseDifference;

                    /* accumulate delta phase to get bin phase */
                    sumPhase_[k] += phase_difference;
                    phase = sumPhase_[k];

                    /* get real and imag part and re-interleave */
                    fftWorkspace_[2 * k] = magnitude * cos(phase);
                    fftWorkspace_[2 * k + 1] = magnitude * sin(phase);
                }

                /* zero negative frequencies */
                for (k = fftFrameSize_ + 2; k < 2* fftFrameSize_; k++) fftWorkspace_[k] = 0.;

                /* do inverse transform */
                fft(fftWorkspace_.data(), fftFrameSize_, true);

                /* do windowing and add to output accumulator */
                for(k = 0; k < fftFrameSize_; k++)
                    outputAccumulator_[k] += 2. * window_[k] * fftWorkspace_[2 * k] / ((fftFrameSize_ / 2) * config::constants::oversamplingFactor);

                for (k = 0; k < stepSize; k++) outFifo_[k] = outputAccumulator_[k];

                /* shift accumulator */
                memmove(outputAccumulator_.data(), outputAccumulator_.data() + stepSize, fftFrameSize_ * sizeof(float));

                /* move input FIFO */
                for (k = 0; k < inFifoLatency; k++) inFifo_[k] = inFifo_[k + stepSize];
            }
        }
    }

    void PitchShifter::fft(float* fftBuffer, long fftFrameSize, bool inverse)
    {
        for (auto k = 0; k < 2*fftFrameSize;k++)
        {
            buffer_[k].real(fftBuffer[2*k]);
            buffer_[k].imag(fftBuffer[2*k+1]);
        }

        fft_->perform(buffer_.data(), buffer_.data(), inverse);

        for (auto k = 0; k < (2 * fftFrameSize); k++)
        {
            fftBuffer[2 * k] = buffer_[k].real();
            fftBuffer[2 * k + 1] = buffer_[k].imag();
        }
    }

    void PitchShifter::setPitchRatio(float pitchRatio)
    {
        pitchFactor_ = pitchRatio;
    }

    void PitchShifter::setFftFrameSize(float fftFrameSize)
    {
        const auto fftFrameSizeIndex = static_cast<int>(fftFrameSize) % config::parameters::fftFrameSizeChoices.size();
        fftFrameSize_ = config::parameters::fftFrameSizeChoices[fftFrameSizeIndex];

        buffer_.resize(2 * fftFrameSize_);

        const auto fftOrder = std::log2(fftFrameSize_);
        fft_ = std::make_unique<juce::dsp::FFT>(static_cast<int>(fftOrder));

        for (auto sa = 0; sa < fftFrameSize_; ++sa)
            window_.push_back(-.5 * cos(2. * M_PI * static_cast<double>(sa) / static_cast<double>(fftFrameSize_)) + .5);
    }
}

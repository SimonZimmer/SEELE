#include <cmath>

#include "PitchShifter.h"
#include "Config.h"
#include "Analysis.h"


namespace hidonash
{
    namespace
    {
        double getWindowFactor(size_t k, size_t windowSize)
        {
            return (-.5 * cos(2. * M_PI * (double)k / (double)windowSize) + .5);
        }
    }

    PitchShifter::PitchShifter(double sampleRate, FactoryPtr factory)
    : factory_(std::move(factory))
    , freqPerBin_(sampleRate / (double)fftFrameSize_)
    , analysis_(factory_->createAnalysis(freqPerBin_))
    , synthesis_(factory_->createSynthesis(freqPerBin_))
    {
        fftFrameSize_ = config::constants::fftFrameSize;
        const auto fftOrder = std::log2(fftFrameSize_);
        fft_ = std::make_unique<juce::dsp::FFT>(static_cast<int>(fftOrder));
        gainCompensation_ = std::pow(10, (65. / 20.));
    }

    void PitchShifter::process(core::AudioBuffer<float>& audioBuffer)
    {
        auto stepSize = fftFrameSize_ / config::constants::oversamplingFactor;
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

                fft_->perform(fftWorkspace_.data(), fftWorkspace_.data(), false);
                analysis_->perform(fftWorkspace_.data());
                synthesis_->perform(fftWorkspace_.data(), *analysis_, pitchFactor_);
                fft_->perform(fftWorkspace_.data(), fftWorkspace_.data(), true);

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

    void PitchShifter::setPitchRatio(float pitchRatio)
    {
        pitchFactor_ = pitchRatio;
    }
}

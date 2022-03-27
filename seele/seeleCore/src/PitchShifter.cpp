#include <cmath>

#include "PitchShifter.h"
#include "Analysis.h"
#include "Synthesis.h"


namespace hidonash
{
    namespace
    {
        double getWindowFactor(size_t sample, size_t windowSize)
        {
            return (-.5 * cos(2. * M_PI * (double)sample / (double)windowSize) + .5);
        }
    }

    PitchShifter::PitchShifter(double sampleRate, FactoryPtr factory)
    : factory_(std::move(factory))
    , freqPerBin_(static_cast<int>(sampleRate / static_cast<double>(config::constants::fftFrameSize)))
    , synthesis_(factory_->createSynthesis(freqPerBin_, factory_->createAnalysis(freqPerBin_)))
    {
        const auto fftOrder = std::log2(constants::fftFrameSize);
        fft_ = std::make_unique<juce::dsp::FFT>(static_cast<int>(fftOrder));
        //TODO: wrap in volume function
        gainCompensation_ = std::pow(10, (65. / 20.));
    }

    //TODO put this in its own class
    void PitchShifter::process(core::AudioBuffer<float>& audioBuffer)
    {
        auto stepSize = fftFrameSize_ / config::constants::oversamplingFactor;
        auto inFifoLatency = fftFrameSize_ - stepSize;
        static long sampleCounter = false;
        if (sampleCounter == false) sampleCounter = inFifoLatency;

        for(auto sa = 0; sa < audioBuffer.getNumSamples(); ++sa)
            audioBuffer[0][sa] = audioBuffer[0][sa] + audioBuffer[1][sa] * 0.5f;

        auto inputData = audioBuffer.getDataPointer();
        auto outputData = audioBuffer.getDataPointer();
        for (auto sa = 0; sa < audioBuffer.getNumSamples(); sa++)
        {
            fifoIn_[sampleCounter] = inputData[sa];
            outputData[sa] = fifoOut_[sampleCounter - inFifoLatency];
            sampleCounter++;

            if (sampleCounter >= constants::fftFrameSize)
            {
                sampleCounter = inFifoLatency;
                pitchShift();

                /* do windowing */
                for (auto k = 0; k < fftFrameSize_;k++)
                {
                    fftWorkspace_[k].real(fifoIn_[k] * getWindowFactor(k, fftFrameSize_));
                    fftWorkspace_[k].imag(0.);
                }

                fft_->perform(fftWorkspace_.data(), fftWorkspace_.data(), false);
                synthesis_->perform(fftWorkspace_.data(), pitchFactor_);
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

        audioBuffer.multiply(gainCompensation_, audioBuffer.getNumSamples());
    }

    void PitchShifter::setPitchRatio(float pitchRatio)
    {
        pitchFactor_ = pitchRatio;
    }
}

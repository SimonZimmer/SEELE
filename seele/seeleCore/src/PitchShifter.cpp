#include <cmath>

#include "PitchShifter.h"
#include "Config.h"
#include "Analysis.h"


namespace hidonash
{
    using namespace config;

    namespace
    {
        double getWindowFactor(size_t k, size_t windowSize)
        {
            return (-.5 * cos(2. * M_PI * (double)k / (double)windowSize) + .5);
        }
    }

    PitchShifter::PitchShifter(double sampleRate, FactoryPtr factory)
    : factory_(std::move(factory))
    , freqPerBin_(static_cast<int>(sampleRate / static_cast<double>(constants::fftFrameSize)))
    , synthesis_(factory_->createSynthesis(freqPerBin_, factory_->createAnalysis(freqPerBin_)))
    {
        const auto fftOrder = std::log2(constants::fftFrameSize);
        fft_ = std::make_unique<juce::dsp::FFT>(static_cast<int>(fftOrder));
        gainCompensation_ = std::pow(10, (65. / 20.));
    }

    void PitchShifter::process(core::AudioBuffer<float>& audioBuffer)
    {
        auto stepSize = constants::fftFrameSize / constants::oversamplingFactor;
        auto inFifoLatency = constants::fftFrameSize - stepSize;
        static long sampleCounter = false;
        if (sampleCounter == false) sampleCounter = inFifoLatency;

        for(auto sa = 0; sa < audioBuffer.getNumSamples(); ++sa)
            audioBuffer[0][sa] = audioBuffer[0][sa] + audioBuffer[1][sa] * 0.5f;

        for (auto sa = 0; sa < audioBuffer.getNumSamples(); sa++)
        {
            /* As long as we have not yet collected enough data just read in */
            fifoIn_[sampleCounter] = audioBuffer.getDataPointer()[sa];
            audioBuffer.getDataPointer()[sa] = fifoOut_[sampleCounter - inFifoLatency];
            sampleCounter++;

            /* now we have enough data for processing */
            if (sampleCounter >= constants::fftFrameSize)
            {
                sampleCounter = inFifoLatency;
                for (auto k = 0; k < constants::fftFrameSize;k++)
                {
                    fftWorkspace_[k].real(fifoIn_[k] * getWindowFactor(k, constants::fftFrameSize));
                    fftWorkspace_[k].imag(0.);
                }

                fft_->perform(fftWorkspace_.data(), fftWorkspace_.data(), false);
                synthesis_->perform(fftWorkspace_.data(), pitchFactor_);
                fft_->perform(fftWorkspace_.data(), fftWorkspace_.data(), true);

                /* do windowing and add to output accumulator */
                for(auto k = 0; k < constants::fftFrameSize; k++)
                    outputAccumulationBuffer_[k] += 2. * getWindowFactor(k, constants::fftFrameSize) * fftWorkspace_[k].real() / ((constants::fftFrameSize / 2) * constants::oversamplingFactor);

                for (auto k = 0; k < stepSize; k++) fifoOut_[k] = outputAccumulationBuffer_[k];

                /* shift accumulator */
                memmove(outputAccumulationBuffer_.data(), outputAccumulationBuffer_.data() + stepSize, constants::fftFrameSize * sizeof(float));
                /* move input FIFO */
                for (auto k = 0; k < inFifoLatency; k++) fifoIn_[k] = fifoIn_[k + stepSize];
            }
        }

        audioBuffer.multiply(gainCompensation_, audioBuffer.getNumSamples() * audioBuffer.getNumChannels());
    }

    void PitchShifter::setPitchRatio(float pitchRatio)
    {
        pitchFactor_ = pitchRatio;
    }
}

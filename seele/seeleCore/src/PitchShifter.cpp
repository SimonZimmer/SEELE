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
    : freqPerBin_(static_cast<int>(sampleRate / static_cast<double>(constants::fftFrameSize)))
    , factory_(std::move(factory))
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
            fifoIn_[sampleCounter] = audioBuffer.getDataPointer()[sa];
            audioBuffer.getDataPointer()[sa] = fifoOut_[sampleCounter - inFifoLatency];
            sampleCounter++;

            if (sampleCounter >= constants::fftFrameSize)
            {
                sampleCounter = inFifoLatency;
                for (auto sa = 0; sa < constants::fftFrameSize; sa++)
                {
                    fftWorkspace_[sa].real(fifoIn_[sa] * getWindowFactor(sa, constants::fftFrameSize));
                    fftWorkspace_[sa].imag(0.);
                }

                fft_->perform(fftWorkspace_.data(), fftWorkspace_.data(), false);
                synthesis_->perform(fftWorkspace_.data(), pitchFactor_);
                fft_->perform(fftWorkspace_.data(), fftWorkspace_.data(), true);

                /* do windowing and add to output accumulator */
                for(auto sa = 0; sa < constants::fftFrameSize; sa++)
                    outputAccumulationBuffer_[sa] += 2. * getWindowFactor(sa, constants::fftFrameSize) * fftWorkspace_[sa].real() / ((constants::fftFrameSize / 2) * constants::oversamplingFactor);

                for (auto sa = 0; sa < stepSize; sa++)
                    fifoOut_[sa] = outputAccumulationBuffer_[sa];

                /* shift accumulator */
                memmove(outputAccumulationBuffer_.data(), outputAccumulationBuffer_.data() + stepSize, constants::fftFrameSize * sizeof(float));
                /* move input FIFO */
                for (auto sa = 0; sa < inFifoLatency; sa++)
                    fifoIn_[sa] = fifoIn_[sa + stepSize];
            }
        }

        audioBuffer.multiply(gainCompensation_, audioBuffer.getNumSamples() * audioBuffer.getNumChannels());
    }

    void PitchShifter::setPitchRatio(float pitchRatio)
    {
        pitchFactor_ = pitchRatio;
    }
}


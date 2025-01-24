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
            return (-0.5 * cos(2.0 * M_PI * static_cast<double>(k) / static_cast<double>(windowSize)) + 0.5);
        }
    }

    PitchShifter::PitchShifter(double sampleRate, IFactory& factory)
    : sampleCounter_(0)
    , stepSize_(config::constants::fftFrameSize / config::constants::oversamplingFactor)
    , inFifoLatency_(config::constants::fftFrameSize - stepSize_)
    , factory_(factory)
    , freqPerBin_(static_cast<int>(sampleRate / static_cast<double>(constants::fftFrameSize)))
    , synthesis_(factory_.createSynthesis(freqPerBin_, factory_.createAnalysis(freqPerBin_)))
    {
        fifoIn_.fill(0.0f);
        fifoOut_.fill(0.0f);

        const auto fftOrder = std::log2(constants::fftFrameSize);
        fft_ = std::make_unique<juce::dsp::FFT>(static_cast<int>(fftOrder));
    }

    void PitchShifter::process(core::IAudioBuffer& audioBuffer)
    {
        const auto numSamples = audioBuffer.getNumSamples();
        const auto fftSize = config::constants::fftFrameSize;

        for (auto sa = 0; sa < numSamples; sa++)
        {
            // Push incoming samples into the FIFO
            fifoIn_[sampleCounter_] = audioBuffer.getDataPointer()[sa];

            // Output samples from the output FIFO if ready
            if (sampleCounter_ >= inFifoLatency_)
            {
                audioBuffer.getDataPointer()[sa] = fifoOut_[sampleCounter_ - inFifoLatency_];
            }

            sampleCounter_++;

            // Process when enough samples are accumulated
            if (sampleCounter_ >= fftSize)
            {
                // 1. Apply window function to the input FIFO
                std::vector<std::complex<float>> fftBuffer(fftSize, {0.0f, 0.0f});
                for (size_t i = 0; i < fftSize; ++i)
                {
                    fftBuffer[i].real(fifoIn_[i] * getWindowFactor(i, fftSize));
                }

                // 2. Perform FFT
                fft_->perform(fftBuffer.data(), fftBuffer.data(), false);

                synthesis_->perform(fftBuffer.data(), pitchFactor_);

                // 3. Perform Inverse FFT
                fft_->perform(fftBuffer.data(), fftBuffer.data(), true);

                // 4. Overlap-add the output
                for (size_t i = 0; i < fftSize; ++i)
                {
                    float windowValue = static_cast<float>(getWindowFactor(i, fftSize));
                    fifoOut_[i] += fftBuffer[i].real() * windowValue / static_cast<float>(fftSize);
                }

                // 5. Shift FIFOs for next frame
                for (size_t i = 0; i < inFifoLatency_; ++i)
                {
                    fifoIn_[i] = fifoIn_[i + stepSize_];
                    fifoOut_[i] = fifoOut_[i + stepSize_];
                }

                // Reset sample counter for overlap-add
                sampleCounter_ = inFifoLatency_;
            }
        }
    }


    void PitchShifter::setPitchRatio(float pitchRatio)
    {
        pitchFactor_ = pitchRatio;
    }
}



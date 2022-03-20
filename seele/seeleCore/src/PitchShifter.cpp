#include <cmath>

#include "PitchShifter.h"
#include "Analysis.h"
#include "Synthesis.h"


namespace hidonash
{
    namespace
    {
        double getWindowFactor(size_t k, size_t windowSize)
        {
            return (-.5 * cos(2. * M_PI * (double)k / (double)windowSize) + .5);
        }
    }

    using namespace config;

    PitchShifter::PitchShifter(double sampleRate, FactoryPtr factory)
    : sampleRate_(sampleRate)
    , freqPerBin_(static_cast<int>(static_cast<float>(sampleRate_) / static_cast<float>(constants::fftFrameSize)))
    , factory_(std::move(factory))
    , analysis_(factory_->createAnalysis(freqPerBin_))
    , synthesis_(factory_->createSynthesis(freqPerBin_))
    {
        const auto fftOrder = std::log2(constants::fftFrameSize);
        fft_ = std::make_unique<juce::dsp::FFT>(static_cast<int>(fftOrder));
        //TODO: wrap in volume function
        gainCompensation_ = std::pow(10, (65. / 20.));
        fftWorkspace_.resize(constants::analysisSize * 2);
    }

    void PitchShifter::process(core::AudioBuffer<float>& audioBuffer)
    {
        auto stepSize = constants::fftFrameSize / constants::oversamplingFactor;
        auto inFifoLatency = constants::fftFrameSize - stepSize;
        static long sampleCounter = false;
        if (sampleCounter == false) sampleCounter = inFifoLatency;

        for(auto sa = 0; sa < audioBuffer.getNumSamples(); ++sa)
            audioBuffer[0][sa] = audioBuffer[0][sa] + audioBuffer[1][sa] * 0.5f;

        auto indata = audioBuffer.getDataPointer();
        auto outdata = audioBuffer.getDataPointer();
        for (auto sa = 0; sa < audioBuffer.getNumSamples(); sa++)
        {
            fifoIn_[sampleCounter] = indata[sa];
            outdata[sa] = fifoOut_[sampleCounter - inFifoLatency];
            sampleCounter++;

            if (sampleCounter >= constants::fftFrameSize)
            {
                sampleCounter = inFifoLatency;

                for (auto k = 0; k < constants::fftFrameSize;k++)
                {
                    fftWorkspace_[k].real(fifoIn_[k] * getWindowFactor(k, constants::fftFrameSize));
                    fftWorkspace_[k].imag(0.f);
                }

                fft_->perform(fftWorkspace_.data(), fftWorkspace_.data(), false);
                analysis_->perform(fftWorkspace_.data());
                const auto analysisMagnitudeBuffer = analysis_->getMagnitudeBuffer();
                const auto analysisFrequencyBuffer = analysis_->getFrequencyBuffer();

                synthesis_->reset();
                auto&& synthesisMagnitudeBuffer = synthesis_->getMagnitudeBuffer();
                auto&& synthesisFrequencyBuffer = synthesis_->getFrequencyBuffer();

                for (auto sa = 0; sa <= constants::fftFrameSize / 2; ++sa)
                {
                    auto index = static_cast<size_t>(sa * pitchFactor_);
                    if (index <= static_cast<size_t>(constants::fftFrameSize / 2))
                    {
                        synthesisMagnitudeBuffer[index] += analysisMagnitudeBuffer[sa];
                        synthesisFrequencyBuffer[index] = analysisFrequencyBuffer[sa] * pitchFactor_;
                    }
                }

                synthesis_->perform(fftWorkspace_.data());
                fft_->perform(fftWorkspace_.data(), fftWorkspace_.data(), true);

                for(auto k = 0; k < constants::fftFrameSize; ++k)
                    outputAccumulationBuffer_[k] += 2.f * getWindowFactor(k, constants::fftFrameSize) * fftWorkspace_[k].real() / ((constants::fftFrameSize / 2) * constants::oversamplingFactor);

                for (auto k = 0; k < stepSize; ++k)
                    fifoOut_[k] = outputAccumulationBuffer_[k];

                memmove(outputAccumulationBuffer_.data(), outputAccumulationBuffer_.data() + stepSize, constants::fftFrameSize * sizeof(float));
                for (auto k = 0; k < inFifoLatency; k++)
                    fifoIn_[k] = fifoIn_[k + stepSize];
            }
        }

        audioBuffer.multiply(gainCompensation_, audioBuffer.getNumSamples());
    }

    void PitchShifter::setPitchRatio(float pitchRatio)
    {
        pitchFactor_ = pitchRatio;
    }
}

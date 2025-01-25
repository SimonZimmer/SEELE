#pragma once

#include <array>

#include <juce_dsp/juce_dsp.h>

#include <core/IAudioBuffer.h>

#include "Config.h"
#include "IFactory.h"


namespace hidonash
{
    class PitchShifter : public IPitchShifter
    {
    public:
        PitchShifter(double sampleRate, IFactory& factory);

        ~PitchShifter() = default;

        PitchShifter& operator=(PitchShifter&& other);

        void process(core::IAudioBuffer::IChannel& channel) override;

        void setPitchRatio(float pitchRatio) override;

    private:
        int freqPerBin_;
        IFactory& factory_;
        SynthesisPtr synthesis_;

        float pitchFactor_;
        double gainCompensation_;

        long sampleCounter_;
        const size_t stepSize_;
        const size_t inFifoLatency_;

        std::unique_ptr<juce::dsp::FFT> fft_;
        std::array<juce::dsp::Complex<float>, 2 * config::constants::analysisSize> fftWorkspace_;

        std::array<float, config::constants::analysisSize> fifoIn_;
        std::array<float, config::constants::analysisSize> fifoOut_;
        std::array<float, 2 * config::constants::analysisSize> outputAccumulationBuffer_;

        std::array<float, config::constants::analysisSize> processedSamples_;
    };
}

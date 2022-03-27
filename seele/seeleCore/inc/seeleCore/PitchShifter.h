#pragma once

#include <array>

#include <juce_dsp/juce_dsp.h>

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"
#include "IAnalysis.h"
#include "IFactory.h"


namespace hidonash
{
    class PitchShifter
    {
    public:
        explicit PitchShifter(double sampleRate, FactoryPtr factory);
        ~PitchShifter() = default;

        void process(core::AudioBuffer<float>& audioBuffer);

        void setPitchRatio(float pitchRatio);

    private:
        int freqPerBin_;
        FactoryPtr factory_;
        SynthesisPtr synthesis_;

        float pitchFactor_{ 0.f };
        size_t fftFrameSize_;
        double gainCompensation_;

        std::array<float, 8192> fifoIn_;
        std::array<float, 8192> fifoOut_;
        std::array<float, 2*8192> outputAccumulationBuffer_;

        std::array<juce::dsp::Complex<float>, 2*8192> fftWorkspace_;
        std::unique_ptr<juce::dsp::FFT> fft_;
    };

}

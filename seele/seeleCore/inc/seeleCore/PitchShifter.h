#pragma once

#include <array>

#include <juce_dsp/juce_dsp.h>

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"
#include "IAnalysis.h"
#include "ISynthesis.h"
#include "Config.h"


namespace hidonash
{
    class PitchShifter
    {
    public:
        explicit PitchShifter(double sampleRate);
        ~PitchShifter() = default;

        void process(core::AudioBuffer<float>& audioBuffer);

        void setPitchRatio(float pitchRatio);

    private:
        AnalysisPtr analysis_;
        SynthesisPtr synthesis_;

        float pitchFactor_{ 0.f };
        double sampleRate_;
        float gainCompensation_;
        int freqPerBin_;

        std::array<float, config::constants::analysisSize> fifoIn_;
        std::array<float, config::constants::analysisSize> fifoOut_;
        std::array<float, 2 * config::constants::analysisSize> outputAccumulationBuffer_;

        std::vector<juce::dsp::Complex<float>> fftWorkspace_;
        std::unique_ptr<juce::dsp::FFT> fft_;
    };

}

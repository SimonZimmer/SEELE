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

        void fft(juce::dsp::Complex<float>* fftBuffer, bool inverse);

    private:
        void synthesis(int freqPerBin, double expectedPhaseDifference);
        FactoryPtr factory_;

        AnalysisPtr analysis_;

        float pitchFactor_{ 0.f };
        double sampleRate_;
        size_t fftFrameSize_;
        double gainCompensation_;
        int freqPerBin_;

        std::array<float, 8192> fifoIn_;
        std::array<float, 8192> fifoOut_;
        std::array<float, 8192/2+1> lastPhase_;
        std::array<float, 8192/2+1> sumPhase_;
        std::array<float, 2*8192> outputAccumulationBuffer_;
        std::array<float, 8192> analysisFrequencyBuffer_;
        std::array<float, 8192> analysisMagnitudeBuffer_;
        std::array<float, 8192> synthesisFrequencyBuffer_;
        std::array<float, 8192> synthesisMagnitudeBuffer_;

        std::array<juce::dsp::Complex<float>, 2*8192> fftWorkspace_;
        std::unique_ptr<juce::dsp::FFT> fft_;
        std::vector<double> window_;
    };

}

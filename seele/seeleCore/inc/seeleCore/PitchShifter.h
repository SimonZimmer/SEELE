#pragma once

#include <array>

#include <juce_dsp/juce_dsp.h>

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"

namespace hidonash
{
    class PitchShifter
    {
    public:
        PitchShifter(double sampleRate);
        ~PitchShifter() = default;

        void process(core::AudioBuffer<float>& audioBuffer);

        void setPitchRatio(float pitchRatio);

        void setFftFrameSize(float fftFrameSize);

        void setOverSamplingFactor(float overSamplingFactor);

        void fft(float *fftBuffer, long fftFrameSize, bool inverse);

        void smbPitchShift(float pitchShift, long numSampsToProcess, long fftFrameSize, long osamp, float *indata, float *outdata);

    private:
        float pitchFactor_{ 0.f };
        int max_frame_length_{ 8192 };
        double sampleRate_;
        size_t fftFrameSize_;

        std::array<float, 8192> fifoIn_;
        std::array<float, 8192> fifoOut_;
        std::array<float, 2*8192> fftWorkspace_;
        std::array<float, 8192/2+1> gLastPhase;
        std::array<float, 8192/2+1> sumPhase_;
        std::array<float, 2*8192> outputAccumulationBuffer_;
        std::array<float, 8192> analysisFrequencyBuffer_;
        std::array<float, 8192> analysisMagnitudeBuffer_;
        std::array<float, 8192> synthesisFrequencyBuffer_;
        std::array<float, 8192> synthesisMagnitudeBuffer_;

        std::vector<juce::dsp::Complex<float>> buffer_;
        std::unique_ptr<juce::dsp::FFT> fft_;
    };

}

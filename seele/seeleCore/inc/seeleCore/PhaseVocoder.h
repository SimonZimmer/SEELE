#pragma once

#include <array>
#include <core/AudioBuffer.h>
#include "IAudioProcessor.h"

namespace hidonash
{
    class PhaseVocoder
    {
    public:
        PhaseVocoder();
        ~PhaseVocoder() = default;

        void process(core::AudioBuffer<float>& audioBuffer);

        void setPitchRatio(float pitchRatio);

        void smbFft(float *fftBuffer, long fftFrameSize, long sign);

        void smbPitchShift(float pitchShift, long numSampsToProcess, long fftFrameSize, long osamp, float sampleRate, float *indata, float *outdata);

    private:
        float pitchRatio_{ 0.f };
        int max_frame_length_{ 8192 };

        std::array<float, 8192> gInFIFO;
        std::array<float, 8192> gOutFIFO;
        std::array<float, 2*8192> gFFTworksp;
        std::array<float, 8192/2+1> gLastPhase;
        std::array<float, 8192/2+1> gSumPhase;
        std::array<float, 2*8192> gOutputAccum;
        std::array<float, 8192> gAnaFreq;
        std::array<float, 8192> gAnaMagn;
        std::array<float, 8192> gSynFreq;
        std::array<float, 8192> gSynMagn;
    };

}

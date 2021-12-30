#pragma once

#include <algorithm>
#include <functional>

#include <juce_dsp/juce_dsp.h>

#include <core/BlockCircularBuffer.h>
#include <core/AudioBuffer.h>
#include "Config.h"
#include "IPhaseCorrector.h"

namespace sz
{
    class PhaseVocoder
    {
    public:
        PhaseVocoder();
        ~PhaseVocoder() = default;

        void updateResampleBufferSize();

        void process(core::AudioBuffer<float>& audioBuffer);

        void setPitchRatio(float pitchRatio);

    private:
        std::unique_ptr<juce::dsp::FFT> fft_;

        BlockCircularBuffer<float> analysisBuffer_;
        BlockCircularBuffer<float> synthesisBuffer_;
        core::AudioBuffer<float> spectralBuffer_{1, config::fft::size};
        core::AudioBuffer<float> resampleBuffer_{1, config::fft::size};

        long incomingSampleCount_ = 0;
        int spectralBufferSize_ = 0;
        int samplesTilNextProcess_ = 0;
        bool isProcessing_ = false;

        std::vector<float> windowFunction_;
        float rescalingFactor_ = 1.f;
        int analysisHopSize_ = 0;
        int synthesisHopSize_ = 0;
        int resampleBufferSize_ = 0;

        float pitchRatio_ = 0.f;
        PhaseCorrectorPtr phaseCorrector_;
    };

}

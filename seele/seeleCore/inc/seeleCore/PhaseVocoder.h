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

        void updatePhaseIncrements(core::AudioBuffer<float>& buffer);

        void setPitchRatio(float newPitchRatio);

    private:
        std::unique_ptr<juce::dsp::FFT> fft;

        // Buffers
        BlockCircularBuffer<float> analysisBuffer;
        BlockCircularBuffer<float> synthesisBuffer;
        core::AudioBuffer<float> spectralBuffer{1, config::fft::size};
        core::AudioBuffer<float> resampleBuffer{1, config::fft::size};

        // Misc state
        long incomingSampleCount = 0;
        int spectralBufferSize = 0;
        int samplesTilNextProcess = 0;
        bool isProcessing = false;

        std::vector<float> windowFunction;
        float rescalingFactor = 1.f;
        int analysisHopSize = 0;
        int synthesisHopSize = 0;
        int resampleBufferSize = 0;

        float pitchRatio = 0.f;
        float timeStretchRatio = 1.f;

        std::vector<float> synthPhaseIncrements;
        std::vector<float> previousFramePhases;
        PhaseCorrectorPtr phaseCorrector_;
    };

}

#pragma once

#include <algorithm>
#include <functional>

#include <core/BlockCircularBuffer.h>
#include <core/AudioBuffer.h>
#include "Config.h"
#include "IAudioProcessor.h"

namespace hidonash
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
        AudioProcessorPtr rescalingProcessor_;

        BlockCircularBuffer<float> analysisBuffer_;
        BlockCircularBuffer<float> synthesisBuffer_;
        core::AudioBuffer<float> spectralBuffer_{1, config::resample::size};
        core::AudioBuffer<float> resampleBuffer_{1, config::resample::size};

        long incomingSampleCount_ = 0;
        int spectralBufferSize_ = 0;
        int samplesTilNextProcess_ = 0;
        bool isProcessing_ = false;

        std::vector<float> windowFunction_;
        int analysisHopSize_ = 0;
        int synthesisHopSize_ = 0;
        int resampleBufferSize_ = 0;

        float pitchRatio_ = 0.f;
    };

}

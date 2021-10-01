#pragma once

#include <algorithm>
#include <functional>

#include <juce_dsp/juce_dsp.h>

#include "core/BlockCircularBuffer.h"
#include <core/AudioBuffer.h>

namespace
{
    using JuceWindow = typename juce::dsp::WindowingFunction<float>;
    using JuceWindowTypes = typename juce::dsp::WindowingFunction<float>::WindowingMethod;

    constexpr float maxPitchRatio = 2.f;
    constexpr float minPitchRatio = 0.5f;
}

namespace sz
{
    class PhaseVocoder
    {
    public:
        PhaseVocoder(int windowLength, int fftSize, JuceWindowTypes windowType);
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
        core::AudioBuffer<float> spectralBuffer{1, 2048};
        core::AudioBuffer<float> resampleBuffer{1, 2048};

        // Misc state
        long incomingSampleCount = 0;
        int spectralBufferSize = 0;
        int samplesTilNextProcess = 0;
        bool isProcessing = false;

        std::vector<float> windowFunction;
        float rescalingFactor = 1.f;
        int analysisHopSize = 0;
        int synthesisHopSize = 0;
        int windowSize = 0;
        int resampleBufferSize = 0;
        int windowOverlaps = 0;

        float pitchRatio = 0.f;
        float timeStretchRatio = 1.f;

        std::vector<float> synthPhaseIncrements;
        std::vector<float> previousFramePhases;
    };

}

#include "PhaseVocoder.h"

#include <algorithm>

#include <juce_dsp/juce_dsp.h>

#include "core/BlockCircularBuffer.h"

namespace hidonash
{
    using namespace config;
    using JuceWindow = typename juce::dsp::WindowingFunction<float>;
    using JuceWindowTypes = typename juce::dsp::WindowingFunction<float>::WindowingMethod;

    namespace
    {
        float interpolate(float sampleA, float sampleB, float fractionIndex)
        {
            return (1.f - fractionIndex) * sampleA + fractionIndex * sampleB;
        }

        void linearResample(const core::AudioBuffer<float>& originalSignal, int originalSignalSize,
                                   core::AudioBuffer<float>& newSignal, int newSignalSize)
        {
            // If the original signal is bigger than the new size, condense the signal to fit the new buffer
            // otherwise expand the signal to fit the new buffer
            const auto scale = static_cast<float>(originalSignalSize) / static_cast<float>(newSignalSize);
            float index = 0.f;

            for (int i = 0; i < newSignalSize; ++i)
            {
                const auto wholeIndex = static_cast<int>(floor(index));
                const auto fractionIndex = index - static_cast<float>(wholeIndex);
                const auto sampleA = originalSignal[0][wholeIndex];
                const auto sampleB = originalSignal[0][wholeIndex + 1];
                newSignal[0][i] = interpolate(sampleA, sampleB, fractionIndex);
                index += scale;
            }
        }
    }

    PhaseVocoder::PhaseVocoder()
    : samplesTilNextProcess_(window::length)
    , resampleBufferSize_(window::length)
    , spectralBufferSize_(window::length * 2)
    , analysisBuffer_(window::length)
    , synthesisBuffer_(window::length * 3)
    , windowFunction_(window::length)
    {
        JuceWindow::fillWindowingTables(windowFunction_.data(), window::length,
                                        JuceWindowTypes::hann, false);

        // Processing reuses the spectral buffer to resize the output grain
        // It must be the at least the size of the min pitch ratio
        spectralBufferSize_ = window::length * (1 / parameters::minPitchRatio) < spectralBufferSize_ ?
                              (int) ceil (window::length * (1 / parameters::minPitchRatio)) : spectralBufferSize_;

        spectralBuffer_.setSize(1, spectralBufferSize_);
        spectralBuffer_.fill(0.f);

        // Calculate maximium size resample signal can be
        const auto maxResampleSize = (int)std::ceil (std::max(window::length * parameters::maxPitchRatio,
                                                                     window::length / parameters::minPitchRatio));

        resampleBuffer_.setSize(1, maxResampleSize);
        resampleBuffer_.fill(0.f);
    }

    void PhaseVocoder::updateResampleBufferSize()
    {
        resampleBufferSize_ = static_cast<int>(std::ceil(window::length * analysisHopSize_ / static_cast<float>(synthesisHopSize_)));
    }

    // The main process function corresponds to the following high level algorithm
    // Note: The processing is split up internally to avoid extra memory usage
    // 1. Read incoming samples into the internal analysis buffer
    // 2. If there are enough samples to begin processing, read a block from the analysis buffer
    // 5. Resample
    // 6. Write the block of samples back into the internal synthesis buffer
    // 7. Read a block of samples from the synthesis buffer
    void PhaseVocoder::process(core::AudioBuffer<float>& audioBuffer)
    {
        const auto audioBufferSize = audioBuffer.getNumSamples();

        // Only write enough samples into the analysis buffer to complete a processing
        // frame. Likewise, only write enough into the synthesis buffer to generate the
        // next output audio frame.
        for (auto internalOffset = 0, internalBufferSize = 0; internalOffset < audioBufferSize; internalOffset += internalBufferSize)
        {
            const auto remainingIncomingSamples = (audioBufferSize - internalOffset);
            internalBufferSize = incomingSampleCount_ + remainingIncomingSamples >= samplesTilNextProcess_ ?
                                 samplesTilNextProcess_ - incomingSampleCount_ : remainingIncomingSamples;

            // Write the incoming samples into the internal buffer
            analysisBuffer_.write(audioBuffer, internalOffset, internalBufferSize);
            incomingSampleCount_ += internalBufferSize;

            // Collected enough samples, do processing
            if (incomingSampleCount_ >= samplesTilNextProcess_)
            {
                isProcessing_ = true;
                incomingSampleCount_ -= samplesTilNextProcess_;

                // After first processing, do another process every analysisHopSize_ samples
                samplesTilNextProcess_ = analysisHopSize_;

                analysisBuffer_.setReadHopSize(analysisHopSize_);
                analysisBuffer_.read(spectralBuffer_, 0, window::length);

                // Apply window to signal
                spectralBuffer_.multiply(windowFunction_, window::length);

                // Resample output grain to N * (hop size analysis / hop size synthesis)
                linearResample(spectralBuffer_, window::length, resampleBuffer_, resampleBufferSize_);

                synthesisBuffer_.setWriteHopSize(synthesisHopSize_);
                synthesisBuffer_.overlapWrite(resampleBuffer_, resampleBufferSize_);
            }

            if (!isProcessing_)
            {
                audioBuffer.fill(0.f);
                continue;
            }

            synthesisBuffer_.read(audioBuffer, internalOffset, internalBufferSize);
        }

        audioBuffer.multiply(1.f /  rescalingFactor_, audioBufferSize);
    }

    void PhaseVocoder::setPitchRatio(float pitchRatio)
    {
        pitchRatio_ = pitchRatio;
        synthesisHopSize_ = static_cast<int>(window::length / static_cast<float>(window::overlaps));
        analysisHopSize_ = static_cast<int>(round(synthesisHopSize_ / pitchRatio_));

        // Rescaling due to OLA processing gain
        double accum = 0.0;

        for (int i = 0; i < window::length; ++i)
            accum += windowFunction_[i] * (double)windowFunction_[i];

        accum /= synthesisHopSize_;
        rescalingFactor_ = static_cast<float>(accum);
        updateResampleBufferSize();
        synthesisHopSize_ = analysisHopSize_;
    }
}

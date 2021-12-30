#include "PhaseVocoder.h"

#include <algorithm>
#include <functional>

#include <juce_dsp/juce_dsp.h>

#include "core/BlockCircularBuffer.h"
#include "PhaseCorrector.h"

namespace sz
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

        // Resample a signal to a new size using linear interpolation
        // The 'originalSize' is the max size of the original signal
        // The 'newSignalSize' is the size to resample to. The 'newSignal' must be at least as big as this size.
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

        int nearestPower2(int value)
        {
            return static_cast<int>(log2(juce::nextPowerOfTwo(value)));
        }
    }

    PhaseVocoder::PhaseVocoder()
    : samplesTilNextProcess_(window::length)
    , resampleBufferSize_(window::length)
    , spectralBufferSize_(window::length * 2)
    , analysisBuffer_(window::length)
    , synthesisBuffer_(window::length * 3)
    , windowFunction_(window::length)
    , fft_(std::make_unique<juce::dsp::FFT>(nearestPower2(fft::size)))
    , phaseCorrector_(std::make_unique<PhaseCorrector>())
    {
        analysisHopSize_ = window::length / window::overlaps;
        synthesisHopSize_ = window::length / window::overlaps;

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
        phaseCorrector_->setTimeStretchRatio(synthesisHopSize_ / static_cast<float>(analysisHopSize_));
    }

    // The main process function corresponds to the following high level algorithm
    // Note: The processing is split up internally to avoid extra memory usage
    // 1. Read incoming samples into the internal analysis buffer
    // 2. If there are enough samples to begin processing, read a block from the analysis buffer
    // 3. Perform an FFT of on the block of samples
    // 4. Do some processing with the spectral data
    // 5. Perform an iFFT back into the time domain
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
            // Once there are enough samples, perform spectral processing
            analysisBuffer_.write(audioBuffer, internalOffset, internalBufferSize);
            incomingSampleCount_ += internalBufferSize;

            // Collected enough samples, do processing
            if (incomingSampleCount_ >= samplesTilNextProcess_)
            {
                isProcessing_ = true;
                incomingSampleCount_ -= samplesTilNextProcess_;

                // After first processing, do another process every analysisHopSize_ samples
                samplesTilNextProcess_ = analysisHopSize_;

                const auto spectralBufferData = spectralBuffer_.getDataPointer();

                analysisBuffer_.setReadHopSize(analysisHopSize_);
                analysisBuffer_.read(spectralBuffer_, 0, window::length);

                // Apply window to signal
                juce::FloatVectorOperations::multiply(spectralBufferData, windowFunction_.data(), window::length);

                // Rotate signal 180 degrees, move the first half to the back and back to the front
                std::rotate(spectralBufferData, spectralBufferData + (window::length / 2), spectralBufferData + window::length);

                // Perform FFT, process and inverse FFT
                fft_->performRealOnlyForwardTransform(spectralBufferData);
                phaseCorrector_->process(spectralBuffer_);
                fft_->performRealOnlyInverseTransform(spectralBufferData);

                // Undo signal back to original rotation
                std::rotate(spectralBufferData, spectralBufferData + (window::length / 2), spectralBufferData + window::length);

                // Apply window to signal
                juce::FloatVectorOperations::multiply(spectralBufferData, windowFunction_.data(), window::length);

                // Resample output grain to N * (hop size analysis / hop size synthesis)
                linearResample(spectralBuffer_, window::length, resampleBuffer_, resampleBufferSize_);

                synthesisBuffer_.setWriteHopSize(synthesisHopSize_);
                synthesisBuffer_.overlapWrite(resampleBuffer_, resampleBufferSize_);
            }

            if (!isProcessing_)
            {
                std::fill(audioBuffer.getDataPointer() + internalOffset,
                          audioBuffer.getDataPointer() + internalOffset + internalBufferSize, 0.f);
                continue;
            }

            synthesisBuffer_.read(audioBuffer, internalOffset, internalBufferSize);
        }

        juce::FloatVectorOperations::multiply(audioBuffer.getDataPointer(), 1.f / rescalingFactor_, audioBufferSize);
    }

    void PhaseVocoder::setPitchRatio(float pitchRatio)
    {
        pitchRatio_ = pitchRatio;
        synthesisHopSize_ = static_cast<int>(window::length / static_cast<float>(window::overlaps));
        analysisHopSize_ = static_cast<int>(round(synthesisHopSize_ / pitchRatio_));
        phaseCorrector_->setHopSize(analysisHopSize_);

        // Rescaling due to OLA processing gain
        double accum = 0.0;

        for (int i = 0; i < window::length; ++i)
            accum += windowFunction_[i] * (double)windowFunction_[i];

        accum /= synthesisHopSize_;
        rescalingFactor_ = static_cast<float>(accum);
        updateResampleBufferSize();
        synthesisHopSize_ = analysisHopSize_;

        phaseCorrector_->resetPhases();
    }

}

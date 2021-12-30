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

        // Principal argument - Unwrap a phase argument to between [-PI, PI]
        float principalArgument(float arg)
        {
            return std::fmod(arg + juce::MathConstants<float>::pi,
                             -juce::MathConstants<float>::twoPi) + juce::MathConstants<float>::pi;
        }
    }

    PhaseVocoder::PhaseVocoder()
    : samplesTilNextProcess(window::length)
    , resampleBufferSize(window::length)
    , spectralBufferSize(window::length * 2)
    , analysisBuffer(window::length)
    , synthesisBuffer(window::length * 3)
    , windowFunction(window::length)
    , fft(std::make_unique<juce::dsp::FFT>(nearestPower2(fft::size)))
    , synthPhaseIncrements(window::length, 0)
    , previousFramePhases(window::length, 0)
    , phaseCorrector_(std::make_unique<PhaseCorrector>())
    {
        analysisHopSize = window::length / window::overlaps;
        synthesisHopSize = window::length / window::overlaps;

        JuceWindow::fillWindowingTables(windowFunction.data(), window::length,
                                        JuceWindowTypes::hann, false);

        // Processing reuses the spectral buffer to resize the output grain
        // It must be the at least the size of the min pitch ratio
        // TODO FFT size must be big enough also
        spectralBufferSize = window::length * (1 / parameters::minPitchRatio) < spectralBufferSize ?
                             (int) ceil (window::length * (1 / parameters::minPitchRatio)) : spectralBufferSize;

        spectralBuffer.setSize(1, spectralBufferSize);
        spectralBuffer.fill(0.f);

        // Calculate maximium size resample signal can be
        const auto maxResampleSize = (int)std::ceil (std::max(window::length * parameters::maxPitchRatio,
                                                                     window::length / parameters::minPitchRatio));

        resampleBuffer.setSize(1, maxResampleSize);
        resampleBuffer.fill(0.f);
    }

    void PhaseVocoder::updateResampleBufferSize()
    {
        resampleBufferSize = static_cast<int>(std::ceil(window::length * analysisHopSize / static_cast<float>(synthesisHopSize)));
        timeStretchRatio = synthesisHopSize / static_cast<float>(analysisHopSize);
        phaseCorrector_->setTimeStretchRatio(synthesisHopSize / static_cast<float>(analysisHopSize));
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
            internalBufferSize = incomingSampleCount + remainingIncomingSamples >= samplesTilNextProcess ?
                                 samplesTilNextProcess - incomingSampleCount : remainingIncomingSamples;

            // Write the incoming samples into the internal buffer
            // Once there are enough samples, perform spectral processing
            analysisBuffer.write(audioBuffer, internalOffset, internalBufferSize);
            incomingSampleCount += internalBufferSize;

            // Collected enough samples, do processing
            if (incomingSampleCount >= samplesTilNextProcess)
            {
                isProcessing = true;
                incomingSampleCount -= samplesTilNextProcess;

                // After first processing, do another process every analysisHopSize samples
                samplesTilNextProcess = analysisHopSize;

                const auto spectralBufferData = spectralBuffer.getDataPointer();

                analysisBuffer.setReadHopSize(analysisHopSize);
                analysisBuffer.read(spectralBuffer, 0, window::length);

                // Apply window to signal
                juce::FloatVectorOperations::multiply(spectralBufferData, windowFunction.data(), window::length);

                // Rotate signal 180 degrees, move the first half to the back and back to the front
                std::rotate(spectralBufferData, spectralBufferData + (window::length / 2), spectralBufferData + window::length);

                // Perform FFT, process and inverse FFT
                fft->performRealOnlyForwardTransform(spectralBufferData);
                phaseCorrector_->process(spectralBuffer);
                fft->performRealOnlyInverseTransform(spectralBufferData);

                // Undo signal back to original rotation
                std::rotate(spectralBufferData, spectralBufferData + (window::length / 2), spectralBufferData + window::length);

                // Apply window to signal
                juce::FloatVectorOperations::multiply(spectralBufferData, windowFunction.data(), window::length);

                // Resample output grain to N * (hop size analysis / hop size synthesis)
                linearResample(spectralBuffer, window::length, resampleBuffer, resampleBufferSize);

                synthesisBuffer.setWriteHopSize(synthesisHopSize);
                synthesisBuffer.overlapWrite(resampleBuffer, resampleBufferSize);
            }

            // Emit silence until we start producing output
            if (!isProcessing)
            {
                std::fill(audioBuffer.getDataPointer() + internalOffset,
                          audioBuffer.getDataPointer() + internalOffset + internalBufferSize, 0.f);
                continue;
            }

            synthesisBuffer.read(audioBuffer, internalOffset, internalBufferSize);
        }

        // Rescale output
        juce::FloatVectorOperations::multiply(audioBuffer.getDataPointer(), 1.f / rescalingFactor, audioBufferSize);
    }

    void PhaseVocoder::setPitchRatio(float newPitchRatio)
    {
        pitchRatio = newPitchRatio;
        synthesisHopSize = (int)(window::length / (float) window::overlaps);
        analysisHopSize = (int)round(synthesisHopSize / pitchRatio);
        phaseCorrector_->setHopSize(analysisHopSize);

        // Rescaling due to OLA processing gain
        double accum = 0.0;

        for (int i = 0; i < window::length; ++i)
            accum += windowFunction[i] * (double)windowFunction[i];

        accum /= synthesisHopSize;
        rescalingFactor = (float)accum;
        updateResampleBufferSize();
        synthesisHopSize = analysisHopSize;

        // Reset phases
        phaseCorrector_->resetPhases();
    }

}

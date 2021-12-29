#include <algorithm>
#include <functional>

#include <juce_dsp/juce_dsp.h>

#include "core/BlockCircularBuffer.h"
#include "PhaseVocoder.h"

namespace sz
{
    namespace
    {
        // Resample a signal to a new size using linear interpolation
        // The 'originalSize' is the max size of the original signal
        // The 'newSignalSize' is the size to resample to. The 'newSignal' must be at least as big as this size.
        void linearResample(const core::AudioBuffer<float>& originalSignal, int originalSignalSize,
                                   core::AudioBuffer<float>& newSignal, int newSignalSize)
        {
            const auto lerp = [&](float v0, float v1, float t) {
                return (1.f - t) * v0 + t * v1;
            };

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
                newSignal[0][i] = lerp(sampleA, sampleB, fractionIndex);
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

    PhaseVocoder::PhaseVocoder(int windowLength, int fftSize, JuceWindowTypes windowType)
    : samplesTilNextProcess(windowLength)
    , windowSize(windowLength)
    , resampleBufferSize(windowLength)
    , spectralBufferSize(windowLength * 2)
    , analysisBuffer(windowLength)
    , synthesisBuffer(windowLength * 3)
    , windowFunction(windowLength)
    , fft(std::make_unique<juce::dsp::FFT>(nearestPower2(fftSize)))
    , synthPhaseIncrements(windowSize, 0)
    , previousFramePhases(windowSize, 0)
    {
        analysisHopSize = windowLength / config::window::overlaps;
        synthesisHopSize = windowLength / config::window::overlaps;

        JuceWindow::fillWindowingTables(windowFunction.data(), windowSize, windowType, false);

        // Processing reuses the spectral buffer to resize the output grain
        // It must be the at least the size of the min pitch ratio
        // TODO FFT size must be big enough also
        spectralBufferSize = windowLength * (1 / minPitchRatio) < spectralBufferSize ?
                             (int) ceil (windowLength * (1 / minPitchRatio)) : spectralBufferSize;

        spectralBuffer.setSize(1, spectralBufferSize);
        spectralBuffer.fill(0.f);

        // Calculate maximium size resample signal can be
        const auto maxResampleSize = (int)std::ceil (std::max (this->windowSize * maxPitchRatio,
                                                               this->windowSize / minPitchRatio));

        resampleBuffer.setSize(1, maxResampleSize);
        resampleBuffer.fill(0.f);
    }

    void PhaseVocoder::updateResampleBufferSize()
    {
        resampleBufferSize = static_cast<int>(std::ceil(windowSize * analysisHopSize / static_cast<float>(synthesisHopSize)));
        timeStretchRatio = synthesisHopSize / static_cast<float>(analysisHopSize);
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
                analysisBuffer.read(spectralBuffer, 0, windowSize);

                // Apply window to signal
                juce::FloatVectorOperations::multiply(spectralBufferData, windowFunction.data(), windowSize);

                // Rotate signal 180 degrees, move the first half to the back and back to the front
                std::rotate(spectralBufferData, spectralBufferData + (windowSize / 2), spectralBufferData + windowSize);

                // Perform FFT, process and inverse FFT
                fft->performRealOnlyForwardTransform(spectralBufferData);
                updatePhaseIncrements(spectralBuffer);
                fft->performRealOnlyInverseTransform(spectralBufferData);

                // Undo signal back to original rotation
                std::rotate(spectralBufferData, spectralBufferData + (windowSize / 2), spectralBufferData + windowSize);

                // Apply window to signal
                juce::FloatVectorOperations::multiply(spectralBufferData, windowFunction.data(), windowSize);

                // Resample output grain to N * (hop size analysis / hop size synthesis)
                linearResample(spectralBuffer, windowSize, resampleBuffer, resampleBufferSize);

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

    void PhaseVocoder::updatePhaseIncrements(core::AudioBuffer<float>& buffer)
    {
        const auto bufferSize = buffer.getNumSamples();
        for (int i = 0, x = 0; i < bufferSize - 1; i += 2, ++x)
        {
            // Update phase increments for pitch shifting
            const auto real = buffer.getDataPointer()[i];
            const auto imag = buffer.getDataPointer()[i + 1];
            const auto mag = sqrtf(real * real + imag * imag);
            const auto phase = atan2(imag, real);

            const auto omega = juce::MathConstants<float>::twoPi * analysisHopSize * x / (float) windowSize;

            const auto deltaPhase = omega + principalArgument(phase - previousFramePhases[x] - omega);

            previousFramePhases[x] = phase;
            synthPhaseIncrements[x] = principalArgument(synthPhaseIncrements[x] +
                                                        (deltaPhase * timeStretchRatio));

            buffer.getDataPointer()[i] = mag * std::cos(synthPhaseIncrements[x]);
            buffer.getDataPointer()[i + 1] = mag * std::sin(synthPhaseIncrements[x]);
        }
    }

    void PhaseVocoder::setPitchRatio(float newPitchRatio)
    {
        pitchRatio = std::clamp(newPitchRatio, minPitchRatio, maxPitchRatio);
        synthesisHopSize = (int)(windowSize / (float) config::window::overlaps);
        analysisHopSize = (int)round(synthesisHopSize / pitchRatio);

        // Rescaling due to OLA processing gain
        double accum = 0.0;

        for (int i = 0; i < windowSize; ++i)
            accum += windowFunction[i] * (double)windowFunction[i];

        accum /= synthesisHopSize;
        rescalingFactor = (float)accum;
        updateResampleBufferSize();
        synthesisHopSize = analysisHopSize;

        // Reset phases
        memset(previousFramePhases.data(), 0, sizeof(float) * windowSize);
        memset(synthPhaseIncrements.data(), 0, sizeof(float) * windowSize);
    }

}

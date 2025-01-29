#pragma once

#include <memory>

#include <core/IAudioBuffer.h>

#include "IAnalysis.h"
#include "IAudioProcessor.h"
#include "IDelayProcessor.h"
#include "IGainProcessor.h"
#include "IMemberParameterSet.h"
#include "IPitchShifter.h"
#include "IPitchShifterManager.h"
#include "ISynthesis.h"


namespace hidonash
{
    class IFactory
    {
    public:
        virtual ~IFactory() = default;

        virtual AudioProcessorPtr createEngine(const IMemberParameterSet& memberParameterSet, double sampleRate,
                                               int samplesPerBlock, size_t numChannels) const = 0;

        virtual AnalysisPtr createAnalysis(int freqPerBin) const = 0;

        virtual SynthesisPtr createSynthesis(int freqPerBin, AnalysisPtr analysis) const = 0;

        virtual PitchShifterPtr createPitchShifter(double sampleRate, IFactory& factory) const = 0;

        virtual PitchShifterManagerPtr createPitchShifterManager(double sampleRate, size_t numChannels,
                                                                 IFactory& factory) const = 0;

        virtual DelayProcessorPtr createDelayProcessor(size_t maxDelaySamples, size_t delaySamples,
                                                       double sampleRate) const = 0;

        virtual GainProcessorPtr createGainProcessor(float gainValue, double sampleRate) const = 0;

        virtual core::AudioBufferPtr createAudioBuffer(int numChannels, int numSamples) const = 0;
    };

    using FactoryPtr = std::unique_ptr<IFactory>;
}

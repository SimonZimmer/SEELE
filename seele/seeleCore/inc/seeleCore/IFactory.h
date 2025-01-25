#pragma once

#include <memory>

#include <core/IAudioBuffer.h>

#include "IAudioProcessor.h"
#include "IAnalysis.h"
#include "ISynthesis.h"
#include "IMemberParameterSet.h"
#include "IPitchShifter.h"
#include "IPitchShifterManager.h"


namespace hidonash
{
    class IFactory
    {
    public:
        virtual ~IFactory() = default;

        virtual AudioProcessorPtr createEngine(const IMemberParameterSet& memberParameterSet, double sampleRate, int samplesPerBlock, size_t numChannels) const = 0;

        virtual AnalysisPtr createAnalysis(int freqPerBin) const = 0;

        virtual SynthesisPtr createSynthesis(int freqPerBin, AnalysisPtr analysis) const = 0;

        virtual PitchShifterPtr createPitchShifter(double sampleRate, IFactory& factory) const = 0;

        virtual PitchShifterManagerPtr createPitchShifterManager(double sampleRate, size_t numChannels, IFactory& factory) const = 0;

        virtual core::AudioBufferPtr createAudioBuffer(int numChannels, int numSamples) const = 0;
    };

    using FactoryPtr = std::unique_ptr<IFactory>;
}


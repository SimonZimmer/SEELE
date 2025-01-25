#include "Factory.h"

#include <core/AudioBuffer.h>

#include "Engine.h"
#include "Analysis.h"
#include "PitchShifterManager.h"
#include "PitchShifter.h"
#include "Synthesis.h"


namespace hidonash
{
    AudioProcessorPtr
    Factory::createEngine(const IMemberParameterSet& memberParameterSet, double sampleRate, int samplesPerBlock) const
    {
        return std::make_unique<Engine>(memberParameterSet, sampleRate, samplesPerBlock);
    }

    AnalysisPtr Factory::createAnalysis(int freqPerBin) const
    {
        return std::make_unique<Analysis>(freqPerBin);
    }

    SynthesisPtr Factory::createSynthesis(int freqPerBin, AnalysisPtr analysis) const
    {
        return std::make_unique<Synthesis>(freqPerBin, std::move(analysis));
    }

    PitchShifterPtr Factory::createPitchShifter(double sampleRate, IFactory& factory) const
    {
        return std::make_unique<PitchShifter>(sampleRate, factory);
    }

    PitchShifterManagerPtr Factory::createPitchShifterManager(double sampleRate, size_t numChannels, IFactory& factory) const
    {
        return std::make_unique<PitchShifterManager>(sampleRate, numChannels, factory);
    }

    core::AudioBufferPtr Factory::createAudioBuffer(int numChannels, int numSamples) const
    {
        return std::make_unique<core::AudioBuffer>(numChannels, numSamples);
    }
}

#pragma once

#include "IFactory.h"


namespace hidonash
{
    class Factory : public IFactory
    {
    public:
        ~Factory() override = default;

        AudioProcessorPtr createEngine(const IMemberParameterSet& memberParameterSet, double sampleRate) const override;

        [[nodiscard]] AnalysisPtr createAnalysis(int freqPerBin) const override;

        [[nodiscard]] SynthesisPtr createSynthesis(int freqPerBin, AnalysisPtr analysis) const override;

        [[nodiscard]] PitchShifterPtr createPitchShifter(double sampleRate, IFactory& factory) const override;

        [[nodiscard]] core::AudioBufferPtr createAudioBuffer(int numChannels, int numSamples) const override;
    };
}

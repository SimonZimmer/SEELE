#pragma once

#include "IFactory.h"

namespace hidonash
{
    class Factory : public IFactory
    {
    public:
        ~Factory() override = default;

        AudioProcessorPtr createEngine(std::atomic<float>& pitchRatio, double sampleRate) const override;

        [[nodiscard]] AnalysisPtr createAnalysis(int freqPerBin) const override;

        [[nodiscard]] SynthesisPtr createSynthesis(int freqPerBin, AnalysisPtr analysis) const override;
    };
}

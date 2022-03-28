#pragma once

#include "IFactory.h"
#include <functional>

namespace hidonash
{
    class Factory : public IFactory
    {
    public:
        ~Factory() override = default;

        AudioProcessorPtr createEngine(std::atomic<float>& seele1Pitch, std::atomic<float>& seele2Pitch, double sampleRate) const override;

        [[nodiscard]] AnalysisPtr createAnalysis(int freqPerBin) const override;

        [[nodiscard]] SynthesisPtr createSynthesis(int freqPerBin, AnalysisPtr analysis) const override;
    };
}

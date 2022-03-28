#include "Factory.h"
#include "Engine.h"
#include "Analysis.h"
#include "Synthesis.h"
#include <functional>

namespace hidonash
{
    AudioProcessorPtr
    Factory::createEngine(std::atomic<float>& seele1Pitch, std::atomic<float>& seele2Pitch, double sampleRate) const
    {
        return std::make_unique<Engine>(seele1Pitch, seele2Pitch, sampleRate);
    }

    AnalysisPtr Factory::createAnalysis(int freqPerBin) const
    {
        return std::make_unique<Analysis>(freqPerBin);
    }

    SynthesisPtr Factory::createSynthesis(int freqPerBin, AnalysisPtr analysis) const
    {
        return std::make_unique<Synthesis>(freqPerBin, std::move(analysis));
    }
}

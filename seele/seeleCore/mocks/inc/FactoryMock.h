#pragma once

#include "seeleCore/IMemberParameterSet.h"
#include <gmock/gmock.h>

#include <seeleCore/IFactory.h>


namespace hidonash
{
    class FactoryMock : public IFactory
    {
    public:
        MOCK_METHOD(AudioProcessorPtr, createEngine, (const IMemberParameterSet&, double), (const));

        MOCK_METHOD(AnalysisPtr, createAnalysis, (int), (const));

        MOCK_METHOD(SynthesisPtr, createSynthesis, (int, AnalysisPtr), (const));
    };
}

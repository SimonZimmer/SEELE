#pragma once

#include <gmock/gmock.h>

#include <seeleCore/IFactory.h>


namespace hidonash
{
    class FactoryMock : public IFactory
    {
    public:
        MOCK_METHOD(AudioProcessorPtr, createEngine, (const IMemberParameterSet&, double, int, size_t), (const));

        MOCK_METHOD(AnalysisPtr, createAnalysis, (int), (const));

        MOCK_METHOD(SynthesisPtr, createSynthesis, (int, AnalysisPtr), (const));

        MOCK_METHOD(PitchShifterPtr, createPitchShifter, (double, IFactory&), (const));

        MOCK_METHOD(PitchShifterManagerPtr, createPitchShifterManager, (double, size_t, IFactory&), (const));

        MOCK_METHOD(core::AudioBufferPtr, createAudioBuffer, (int, int), (const));
    };
}


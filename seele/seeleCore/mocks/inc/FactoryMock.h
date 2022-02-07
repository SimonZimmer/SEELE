#pragma once

#include <gmock/gmock.h>

#include <seeleCore/IFactory.h>

namespace hidonash
{
    class FactoryMock : public IFactory
    {
    public:
        MOCK_CONST_METHOD1(createEngine, AudioProcessorPtr(std::atomic<float>&))

        AudioProcessorPtr createEngine(std::atomic<float>& gmock_a0, size_t sampleRate)

        ;
    };
}

#pragma once

#include <gmock/gmock.h>

#include <seeleCore/IFactory.h>

namespace hidonash
{
    class FactoryMock : public IFactory
    {
    public:
        MOCK_METHOD(AudioProcessorPtr, createEngine, (std::atomic<float>&, std::atomic<float>&, double), (const));
    };
}

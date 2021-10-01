#pragma once

#include <gmock/gmock.h>

#include <seeleCore/IFactory.h>

namespace sz
{
    class FactoryMock : public IFactory
    {
    public:
        MOCK_CONST_METHOD1(createChopper, AudioProcessorPtr(std::atomic<float>&));
        MOCK_CONST_METHOD2(createEngine, AudioProcessorPtr(std::atomic<float>&,
                                                           std::atomic<float>&));
    };
}

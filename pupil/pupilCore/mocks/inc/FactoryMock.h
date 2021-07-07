#pragma once

#include <gmock/gmock.h>

#include <pupilCore/IFactory.h>

namespace sz
{
    class FactoryMock : public IFactory
    {
    public:
        MOCK_CONST_METHOD1(createChopper, AudioProcessorPtr(std::atomic<float>&));
        MOCK_CONST_METHOD1(createEngine, AudioProcessorPtr(std::atomic<float>&));
    };
}

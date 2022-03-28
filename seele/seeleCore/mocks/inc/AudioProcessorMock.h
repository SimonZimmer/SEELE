#pragma once

#include <gmock/gmock.h>

#include <seeleCore/IAudioProcessor.h>


namespace hidonash
{
    class AudioProcessorMock : public IAudioProcessor
    {
    public:
        MOCK_METHOD(void, process, (core::IAudioBuffer&));
    };

}

#pragma once

#include <gmock/gmock.h>

#include <seeleCore/IGainProcessor.h>


namespace hidonash
{
    class GainProcessorMock : public IGainProcessor
    {
    public:
        ~GainProcessorMock() = default;

        MOCK_METHOD(void, setGainDb, (float) );

        MOCK_METHOD(void, process, (core::IAudioBuffer::IChannel&) );
    };
}

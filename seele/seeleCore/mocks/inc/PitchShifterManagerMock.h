#pragma once

#include <gmock/gmock.h>

#include <seeleCore/IPitchShifterManager.h>


namespace hidonash
{
    class PitchShifterManagerMock : public IPitchShifterManager
    {
    public:
        ~PitchShifterManagerMock() = default;

        MOCK_METHOD(void, setPitchRatio, (float) );

        MOCK_METHOD(void, process, (core::IAudioBuffer&) );
    };
}

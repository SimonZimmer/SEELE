
#pragma once

#include <gmock/gmock.h>

#include <seeleCore/IPitchShifter.h>


namespace hidonash
{
    class PitchShifterMock : public IPitchShifter
    {
    public:
        ~PitchShifterMock() = default;

        MOCK_METHOD(void, setPitchRatio, (float));

        MOCK_METHOD(void, process, (core::IAudioBuffer&));
    };

}


#pragma once

#include <gmock/gmock.h>

#include <futakuchiCore/ISampleRamp.h>

namespace sz
{
    class SampleRampMock : public ISampleRamp
    {
    public:
        MOCK_METHOD0(getNextGain, float());
    };
}

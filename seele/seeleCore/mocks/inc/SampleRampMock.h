#pragma once

#include <gmock/gmock.h>

#include <seele/ISampleRamp.h>

namespace sz
{
    class SampleRampMock : public ISampleRamp
    {
    public:
        MOCK_METHOD0(getNextGain, float());
    };
}

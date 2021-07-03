#pragma once

#include <gmock/gmock.h>

#include <pupilCore/IAudioProcessor.h>

namespace sz
{
    class AudioProcessorMock : public IAudioProcessor
    {
    public:
        MOCK_METHOD1(process, void(core::AudioBuffer<float>&));
    };

}

#pragma once

#include <memory>

#include <core/IAudioBuffer.h>


namespace hidonash
{
    class IChannelProcessor
    {
    public:
        virtual ~IChannelProcessor() = default;

        virtual void process(core::IAudioBuffer::IChannel& channel) = 0;
    };

    using ChannelProcessorPtr = std::unique_ptr<IChannelProcessor>;
}

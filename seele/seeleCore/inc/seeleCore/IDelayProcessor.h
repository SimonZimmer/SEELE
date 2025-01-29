#pragma once

#include <memory>

#include "IChannelProcessor.h"


namespace hidonash
{
    class IDelayProcessor : public IChannelProcessor
    {
    public:
        virtual void setDelayInSamples(int delayInSamples) = 0;
    };

    using DelayProcessorPtr = std::unique_ptr<IDelayProcessor>;
}

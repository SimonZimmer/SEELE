#pragma once

#include <memory>

#include "DelayProcessor.h"
#include "IChannelProcessor.h"


namespace hidonash
{
    class IDelayProcessor : public IChannelProcessor
    {
    public:
        virtual void setDelayInSamples(size_t delayInSamples) = 0;
    };

    using DelayProcessorPtr = std::unique_ptr<DelayProcessor>;
}


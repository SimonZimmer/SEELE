#pragma once

#include <memory>

#include "IChannelProcessor.h"


namespace hidonash
{
    class IGainProcessor : public IChannelProcessor
    {
    public:
        virtual void setGainDb(float gainDb) = 0;
    };

    using GainProcessorPtr = std::unique_ptr<IGainProcessor>;
}

#pragma once

#include <core/AudioBuffer.h>
#include <core/InterpolatingValue.h>

#include "IGainProcessor.h"


namespace hidonash
{
    class GainProcessor : public IGainProcessor
    {
    public:
        GainProcessor(float gainValueDb, double sampleRate);

        void process(core::IAudioBuffer::IChannel& input) override;

        void setGainDb(float gainDb) override;

    private:
        core::InterpolatingValue<float> gainValue_;
    };
}

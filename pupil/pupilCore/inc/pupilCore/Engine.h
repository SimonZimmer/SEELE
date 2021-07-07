#pragma once

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"
#include "IFactory.h"
#include "Factory.h"

namespace sz
{
    class Engine : public IAudioProcessor
    {
    public:
        Engine(std::atomic<float>& chopFrequency,
               const IFactory& factory = Factory());

        ~Engine() = default;

        void process(core::AudioBuffer<float>& inputBuffer) override;

    private:
        const IFactory& factory_;
        AudioProcessorPtr chopper_;
    };
}

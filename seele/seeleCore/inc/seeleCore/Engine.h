#pragma once

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"
#include "IFactory.h"
#include "Factory.h"
#include "PitchShifter.h"
#include "Config.h"

namespace hidonash
{
    class Engine : public IAudioProcessor
    {
    public:
        Engine(std::atomic<float>& pitchRatio, double sampleRate, FactoryPtr factory = std::make_unique<Factory>());

        ~Engine() = default;

        void process(core::AudioBuffer<float>& inputBuffer) override;

    private:
        PitchShifter pitchShifter;
        std::atomic<float>& pitchRatio_;
    };
}

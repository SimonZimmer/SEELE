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
        Engine(std::atomic<float>& pitchRatio, std::atomic<float>& fftFrameSize, double sampleRate,
               const IFactory& factory = Factory());

        ~Engine() = default;

        void process(core::AudioBuffer<float>& inputBuffer) override;

    private:
        PitchShifter pitchShifter;
        std::atomic<float>& pitchRatio_;
        std::atomic<float>& fftFrameSize_;
    };
}

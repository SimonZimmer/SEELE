#pragma once

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"
#include "IFactory.h"
#include "Factory.h"
#include "PitchShifter.h"

namespace sz
{
    class Engine : public IAudioProcessor
    {
    public:
        Engine(std::atomic<float>& chopFrequency,
               std::atomic<float>& pitchRatio,
               const IFactory& factory = Factory());

        ~Engine() = default;

        void process(core::AudioBuffer<float>& inputBuffer) override;

    private:
        const IFactory& factory_;
        AudioProcessorPtr chopper_;
        PitchShifter<float> pitchShifter_{};
        juce::AudioBuffer<float> buffer_;
        std::atomic<float>& pitchRatio_;
    };
}

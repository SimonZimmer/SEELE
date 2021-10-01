#pragma once

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"
#include "IFactory.h"
#include "Factory.h"
#include "PhaseVocoder.h"

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
        PhaseVocoder phaseVocoder_{2048, 2048, JuceWindowTypes::hann};
        std::atomic<float>& pitchRatio_;
    };
}

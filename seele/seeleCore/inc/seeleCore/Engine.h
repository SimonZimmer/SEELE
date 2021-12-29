#pragma once

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"
#include "IFactory.h"
#include "Factory.h"
#include "PhaseVocoder.h"
#include "Config.h"

namespace sz
{
    class Engine : public IAudioProcessor
    {
    public:
        Engine(std::atomic<float>& pitchRatio,
               const IFactory& factory = Factory());

        ~Engine() = default;

        void process(core::AudioBuffer<float>& inputBuffer) override;

    private:
        const IFactory& factory_;
        PhaseVocoder phaseVocoder_{config::fft::size,
                                   config::fft::size,
                                   JuceWindowTypes::hann};
        std::atomic<float>& pitchRatio_;
    };
}

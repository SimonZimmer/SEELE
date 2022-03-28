#pragma once

#include <array>
#include <functional>
#include <vector>

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
        Engine(std::atomic<float>& seele1Pitch_, std::atomic<float>& seele2Pitch_, double sampleRate, FactoryPtr factory = std::make_unique<Factory>());

        ~Engine() = default;

        void process(core::IAudioBuffer& inputBuffer) override;

    private:
        std::vector<std::unique_ptr<PitchShifter>> pitchShifters_;
        std::vector<std::unique_ptr<core::AudioBuffer>> audioBuffers_;
        std::atomic<float>& seele1Pitch_;
        std::atomic<float>& seele2Pitch_;
    };
}


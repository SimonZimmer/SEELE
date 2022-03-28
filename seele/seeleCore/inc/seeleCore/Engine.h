#pragma once

#include <array>
#include <functional>
#include <vector>

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"
#include "IFactory.h"
#include "IMemberParameterSet.h"
#include "Factory.h"
#include "PitchShifter.h"
#include "Config.h"
#include "seeleCore/IMemberParameterSet.h"

namespace hidonash
{
    class Engine : public IAudioProcessor
    {
    public:
        Engine(const IMemberParameterSet& memberParameterSet, double sampleRate, FactoryPtr factory = std::make_unique<Factory>());

        ~Engine() = default;

        void process(core::IAudioBuffer& inputBuffer) override;

    private:
        std::vector<std::unique_ptr<PitchShifter>> pitchShifters_;
        std::vector<std::unique_ptr<core::AudioBuffer>> audioBuffers_;
        const IMemberParameterSet& memberParameterSet_;
    };
}


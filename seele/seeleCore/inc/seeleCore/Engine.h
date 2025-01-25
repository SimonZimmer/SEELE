#pragma once

#include <vector>

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"
#include "IFactory.h"
#include "IMemberParameterSet.h"
#include "IPitchShifter.h"
#include "Factory.h"
#include "Config.h"


namespace hidonash
{
    class Engine : public IAudioProcessor
    {
    public:
        Engine(const IMemberParameterSet& memberParameterSet, double sampleRate, int samplesPerBlock,
               FactoryPtr factory = std::make_unique<Factory>(),
               size_t numMembers = config::constants::numMembers);

        ~Engine() = default;

        void process(core::IAudioBuffer& inputBuffer) override;

    private:
        std::vector<PitchShifterPtr> pitchShifters_;
        std::vector<core::AudioBufferPtr> audioBuffers_;
        const IMemberParameterSet& memberParameterSet_;
        size_t numMembers_;
    };
}


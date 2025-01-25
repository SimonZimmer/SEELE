#pragma once

#include <vector>

#include <core/AudioBuffer.h>

#include "IFactory.h"
#include "IMemberParameterSet.h"
#include "IPitchShifterManager.h"
#include "Factory.h"


namespace hidonash
{
    class Engine : public IAudioProcessor
    {
    public:
        Engine(const IMemberParameterSet& memberParameterSet, double sampleRate,
               int samplesPerBlock, size_t numChannels,
               FactoryPtr factory = std::make_unique<Factory>(),
               size_t numMembers = config::constants::numMembers);

        ~Engine() = default;

        void process(core::IAudioBuffer& inputBuffer) override;

    private:
        std::vector<PitchShifterManagerPtr> pitchShifterManagers_;
        std::vector<core::AudioBufferPtr> audioBuffers_;
        const IMemberParameterSet& memberParameterSet_;
        size_t numMembers_;
    };
}


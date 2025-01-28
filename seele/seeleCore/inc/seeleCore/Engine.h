#pragma once

#include <vector>

#include <core/AudioBuffer.h>

#include "IFactory.h"
#include "Factory.h"
#include "IMemberParameterSet.h"
#include "IPitchShifterManager.h"
#include "IDelayProcessor.h"


namespace hidonash
{
    class Engine : public IAudioProcessor
    {
    public:
        Engine(const IMemberParameterSet& memberParameterSet, double sampleRate,
               int samplesPerBlock, size_t numChannels,
               FactoryPtr factory = std::make_unique<Factory>());

        ~Engine() = default;

        void process(core::IAudioBuffer& inputBuffer) override;

    private:
        std::vector<PitchShifterManagerPtr> pitchShifterManagers_;
        std::vector<DelayProcessorPtr> delayProcessorsLeft_;
        std::vector<DelayProcessorPtr> delayProcessorsRight_;
        std::vector<core::AudioBufferPtr> audioBuffers_;
        const IMemberParameterSet& memberParameterSet_;
        core::AudioBuffer internalBuffer_;
        size_t numChannels_;
        double sampleRate_;
        std::vector<float> lastDistances_;
    };
}

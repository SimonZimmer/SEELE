#include "Engine.h"

#include "IGainProcessor.h"
#include "core/AudioBuffer.h"

#include "Config.h"
#include "IMemberParameterSet.h"


namespace hidonash
{
    namespace
    {
        int convertMsToSamples(const int ms, double sampleRate)
        {
            if (ms >= 0)
                return 0;

            return std::floor(static_cast<float>(ms / 1000.f * sampleRate));
        }
    }

    Engine::Engine(const IMemberParameterSet& memberParameterSet, double sampleRate, int samplesPerBlock,
                   size_t numChannels, FactoryPtr factory)
    : memberParameterSet_(memberParameterSet)
    , internalBuffer_(numChannels, samplesPerBlock)
    , numChannels_(numChannels)
    , sampleRate_(sampleRate)
    {
        for (auto n = 0; n < config::constants::numMembers; ++n)
        {
            pitchShifterManagers_.emplace_back(factory->createPitchShifterManager(sampleRate, numChannels_, *factory));
            audioBuffers_.emplace_back(factory->createAudioBuffer(numChannels_, samplesPerBlock));
            delayProcessorsLeft_.emplace_back(factory->createDelayProcessor(100000.0f, 0.0f, sampleRate));
            delayProcessorsRight_.emplace_back(factory->createDelayProcessor(100000.0f, 0.0f, sampleRate));
            gainProcessorsLeft_.emplace_back(factory->createGainProcessor(0.0f, sampleRate));
            gainProcessorsRight_.emplace_back(factory->createGainProcessor(0.0f, sampleRate));
        }
    }

    void Engine::process(core::IAudioBuffer& inputBuffer)
    {
        internalBuffer_.fill(0.0f);

        int activeMembers = 0;
        for (auto n = 0; n < config::constants::numMembers; ++n)
            if (memberParameterSet_.getSummonState(n))
            {
                audioBuffers_[n]->copyFrom(inputBuffer);
                pitchShifterManagers_[n]->setPitchRatio(memberParameterSet_.getSanctity(n));
                pitchShifterManagers_[n]->process(*audioBuffers_[n]);
                delayProcessorsLeft_[n]->setDelayInSamples(std::floor(memberParameterSet_.getDistance(n)));
                delayProcessorsRight_[n]->setDelayInSamples(std::floor(memberParameterSet_.getDistance(n)));
                delayProcessorsLeft_[n]->process(*audioBuffers_[n]->getChannel(0));
                delayProcessorsRight_[n]->process(*audioBuffers_[n]->getChannel(1));
                gainProcessorsLeft_[n]->setGainDb(memberParameterSet_.getGain(n));
                gainProcessorsRight_[n]->setGainDb(memberParameterSet_.getGain(n));
                gainProcessorsLeft_[n]->process(*audioBuffers_[n]->getChannel(0));
                gainProcessorsRight_[n]->process(*audioBuffers_[n]->getChannel(1));
                internalBuffer_.add(*audioBuffers_[n], inputBuffer.getNumSamples());
                activeMembers++;
            }

        if (activeMembers <= 0)
            return;

        internalBuffer_.multiply(1.f / static_cast<float>(activeMembers), internalBuffer_.getNumSamples());

        inputBuffer.copyFrom(internalBuffer_);
    }
}

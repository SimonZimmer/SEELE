#include "RescalingProcessor.h"

#include "Config.h"

namespace hidonash
{
    using namespace config;

    RescalingProcessor::RescalingProcessor(std::vector<float>& windowFunction)
    : windowFunction_(windowFunction)
    {
        double accum = 0.0;

        for (int i = 0; i < window::length; ++i)
            accum += windowFunction_[i] * (double)windowFunction_[i];

        const auto synthesisHopSize = static_cast<int>(window::length / static_cast<float>(window::overlaps));
        accum /= synthesisHopSize;
        rescalingFactor_ = 1 / static_cast<float>(accum);
    }

    void RescalingProcessor::process(core::AudioBuffer<float>& inputBuffer)
    {
        inputBuffer.multiply(rescalingFactor_, inputBuffer.getNumSamples() * inputBuffer.getNumChannels());
    }
}

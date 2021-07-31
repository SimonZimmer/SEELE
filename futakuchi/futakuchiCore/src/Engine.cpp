#include "Engine.h"

namespace sz
{
    Engine::Engine(std::atomic<float>& chopFrequency,
                   std::atomic<float>& pitchRatio,
                   const IFactory& factory)
    : factory_(factory)
    , chopper_(factory_.createChopper(chopFrequency))
    , pitchRatio_(pitchRatio)
    {
        buffer_ = juce::AudioBuffer<float>(2, 512);
        pitchShifter_.setPitchRatio(pitchRatio);
    }

    void Engine::process(core::AudioBuffer<float>& inputBuffer)
    {
        buffer_.setSize(2, 512);

        for (auto ch = 0; ch < buffer_.getNumChannels(); ++ch)
            for (auto sa = 0; sa < buffer_.getNumSamples(); ++sa)
                buffer_.setSample(ch, sa, inputBuffer[ch][sa]);

        pitchShifter_.setPitchRatio(pitchRatio_);
        pitchShifter_.process(*buffer_.getArrayOfWritePointers(), inputBuffer.getNumChannels() * inputBuffer.getNumSamples());

        for (auto ch = 0; ch < buffer_.getNumChannels(); ++ch)
            for (auto sa = 0; sa < buffer_.getNumSamples(); ++sa)
                inputBuffer.setSample(ch, sa, buffer_.getSample(ch, sa));

        chopper_->process(inputBuffer);
    }
}

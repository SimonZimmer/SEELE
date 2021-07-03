#include "DelayProcessor.h"

#include <iostream>

namespace sz
{
    DelayProcessor::DelayProcessor(std::atomic<float>& parameterValue)
    : parameterValue_(parameterValue)
    {
    }

    void DelayProcessor::process(core::AudioBuffer<float>& inputBuffer)
    {
        std::cout << parameterValue_ << std::endl;

        for(auto ch{0}; ch < inputBuffer.getNumChannels(); ++ch)
            for(auto sa{0}; sa < inputBuffer.getNumSamples(); ++sa)
                inputBuffer.setSample(ch, sa, parameterValue_ * inputBuffer.getSample(ch, sa));
    }
}

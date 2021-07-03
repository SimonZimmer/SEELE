#include "DelayProcessor.h"

namespace sz
{
    DelayProcessor::DelayProcessor(float& parameterValue)
    : parameterValue_(parameterValue)
    {
    }

    void DelayProcessor::process(core::AudioBuffer<float>& inputBuffer)
    {
        for(auto ch{0}; ch < inputBuffer.getNumChannels(); ++ch)
            for(auto sa{0}; sa < inputBuffer.getNumChannels(); ++sa)
                inputBuffer.setSample(ch, sa, parameterValue_);
    }
}

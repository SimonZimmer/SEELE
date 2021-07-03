#include "Chopper.h"

#include <cmath>

namespace sz
{
    Chopper::Chopper(std::atomic<float>& parameterValue, std::atomic<float>& chopGain)
    : parameterValue_(parameterValue)
    , chopGain_(chopGain)
    {
    }

    void Chopper::process(core::AudioBuffer<float>& inputBuffer)
    {
        const auto numSamples = inputBuffer.getNumSamples();
        const auto chopLength = std::max(1.f, std::floor(parameterValue_ * numSamples));
        const auto padding = std::max(1.f, std::floor(parameterValue_ * chopLength));

        for(auto ch = 0; ch < inputBuffer.getNumChannels(); ++ch)
        {
            auto offset = 1;
            while(offset < numSamples)
            {
                for (auto sa = offset; sa <= padding; ++sa)
                    inputBuffer.setSample(ch, sa, inputBuffer[ch][sa] * chopGain_);
                offset += chopLength;
            }
        }
    }
}

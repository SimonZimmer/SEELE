#include "AmplitudeModulation.h"
#include "SampleRamp.h"

#include <cmath>

namespace sz
{
    using namespace std::chrono_literals;

    AmplitudeModulation::AmplitudeModulation(std::atomic<float>& chopFrequency)
    : chopFrequency_(chopFrequency)
    , startValue_(0.f)
    {
    }

    void AmplitudeModulation::process(core::AudioBuffer<float>& inputBuffer)
    {
        double currentSampleRate = 44100.0;
        auto currentAngle = 0.0;
        auto cyclesPerSample = chopFrequency_ / currentSampleRate;
        auto angleDelta = cyclesPerSample * 2.0 * M_PI;

        for (auto sa = 0; sa <= inputBuffer.getNumSamples(); ++sa)
        {
            auto currentSample = std::abs(static_cast<float>(std::sin(currentAngle)));
            currentAngle += angleDelta;
            for(auto ch = 0; ch < inputBuffer.getNumChannels(); ++ch)
                inputBuffer[ch][sa] = currentSample * inputBuffer[ch][sa];
        }
    }
}

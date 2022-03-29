#pragma once

#include <array>


namespace hidonash
{
    namespace config
    {
        namespace parameters
        {
            constexpr auto maxPitchFactor = 2.f;
            constexpr auto minPitchFactor = 0.4f;
            constexpr auto defaultPitchFactor = 1.f;
        }

        namespace constants
        {
            constexpr auto fftFrameSize = 4096;
            constexpr auto analysisSize = 2 * fftFrameSize;
            constexpr auto oversamplingFactor = 64;
            constexpr auto stepSize = fftFrameSize / config::constants::oversamplingFactor;
            constexpr auto expectedPhaseDifference = 2. * M_PI * (double)stepSize / (double)fftFrameSize;
            constexpr auto numMembers = 3;
        }
    }
}


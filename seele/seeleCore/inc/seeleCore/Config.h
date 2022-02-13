#pragma once

#include <array>

namespace hidonash
{
    namespace config
    {
        namespace constants
        {
            constexpr auto oversamplingFactor = 32;
        }

        namespace parameters
        {
            constexpr auto maxPitchFactor = 2.f;
            constexpr auto minPitchFactor = 0.4f;
            constexpr auto defaultPitchFactor = 1.f;

            constexpr auto fftFrameSizeChoices = std::array<size_t, 7>{ 32, 64, 128, 256, 512, 1024, 2048 };
        }
    }
}

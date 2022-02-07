#pragma once

#include <array>

namespace hidonash
{
    namespace config
    {
        namespace resample
        {
            constexpr auto size = 8192;
        }

        namespace window
        {
            constexpr auto overlaps = 4;
            constexpr auto length = resample::size;
        }

        namespace parameters
        {
            constexpr auto maxPitchFactor = 2.f;
            constexpr auto minPitchFactor = 0.4f;
            constexpr auto defaultPitchFactor = 1.f;

            constexpr auto fftFrameSizeChoices = std::array<size_t, 8>{ 32, 64, 128, 256, 512, 1024, 2048, 4096 };
        }
    }
}

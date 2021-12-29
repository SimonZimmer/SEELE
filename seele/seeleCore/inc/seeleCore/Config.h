#pragma once

namespace sz
{
    namespace config
    {
        namespace fft
        {
            constexpr auto size = 8192;
        }

        namespace window
        {
            constexpr auto overlaps = 8;
            constexpr auto length = fft::size;
        }

        namespace parameters
        {
            constexpr auto maxPitchRatio = 2.f;
            constexpr auto minPitchRatio = 0.4f;
        }
    }
}

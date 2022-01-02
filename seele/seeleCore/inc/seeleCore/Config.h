#pragma once

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
            constexpr auto maxPitchRatio = 2.f;
            constexpr auto minPitchRatio = 0.4f;
            constexpr auto defaultPitchRatio = 1.f;
        }
    }
}

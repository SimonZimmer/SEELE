#pragma once

#include <core/IAudioBuffer.h>

#include "IFactory.h"


namespace hidonash
{
    class PitchShifterManager : public IPitchShifterManager
    {
    public:
        PitchShifterManager(double sampleRate, size_t numChannels, IFactory& factory);

        ~PitchShifterManager() = default;

        void process(core::IAudioBuffer& audioBuffer) override;

        void setPitchRatio(float pitchRatio) override;

    private:
        std::vector<PitchShifterPtr> pitchShifters_;
        float pitchRatio_;
    };
}

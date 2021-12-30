#include <core/AudioBuffer.h>

#include "IPhaseCorrector.h"
#include "Config.h"

namespace sz
{
    class PhaseCorrector : public IPhaseCorrector
    {
    public:
        PhaseCorrector();
        ~PhaseCorrector() override = default;

        void process(core::AudioBuffer<float>& inputBuffer) override;

        void setHopSize(size_t hopSize) override;

        void setTimeStretchRatio(float ratio) override;

        void resetPhases() override;

    private:
        std::vector<float> synthPhaseIncrements_{ config::window::length, 0 };
        std::vector<float> previousFramePhases_{ config::window::length, 0 };
        size_t analysisHopSize_{ 0 };
        float timeStretchRatio_{ 1.f };
    };
}

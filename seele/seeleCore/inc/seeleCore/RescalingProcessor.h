#pragma once

#include <core/AudioBuffer.h>

#include "IAudioProcessor.h"
#include "Config.h"

namespace hidonash
{
    class RescalingProcessor : public IAudioProcessor
    {
    public:
        RescalingProcessor(std::vector<float>& windowFunction);

        ~RescalingProcessor() override = default;

        void process(core::AudioBuffer<float>& inputBuffer) override;

    private:
        float rescalingFactor_{ 1.f };
        std::vector<float> windowFunction_;
    };
}

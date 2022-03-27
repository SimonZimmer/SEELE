#pragma once

#include <memory>
#include <array>
#include <juce_dsp/juce_dsp.h>

#include "Config.h"
#include "IAnalysis.h"

namespace hidonash
{
    class ISynthesis
    {
    public:
        virtual ~ISynthesis() = default;

        virtual void perform(juce::dsp::Complex<float>* fftWorkspace, const IAnalysis& analysis, float pitchFactor) = 0;
    };

    using SynthesisPtr = std::unique_ptr<ISynthesis>;
}

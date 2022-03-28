#pragma once

#include <atomic>
#include <array>

#include <juce_audio_processors/juce_audio_processors.h>

#include "Config.h"
#include "IMemberParameterSet.h"


namespace hidonash
{
    class MemberParameterSet : public IMemberParameterSet
    {
    public:
        MemberParameterSet(const juce::AudioProcessorValueTreeState& apts);
        ~MemberParameterSet() override = default;

        float getPitchRatio(size_t index) const override;

    private:
        const juce::AudioProcessorValueTreeState& apts_;
    };

}


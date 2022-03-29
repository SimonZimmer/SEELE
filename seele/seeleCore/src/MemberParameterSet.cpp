#include <string>

#include "MemberParameterSet.h"


namespace hidonash
{
    MemberParameterSet::MemberParameterSet(const juce::AudioProcessorValueTreeState& apts)
    : apts_(apts)
    {
    }

    float MemberParameterSet::getPitchRatio(size_t index) const
    {
        const auto parameterId = "seele" + std::to_string(index);
        return apts_.getRawParameterValue(parameterId)->load();
    }
}


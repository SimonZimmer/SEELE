#include <string>

#include "MemberParameterSet.h"
#include "Config.h"


namespace hidonash
{
    MemberParameterSet::MemberParameterSet(const juce::AudioProcessorValueTreeState& apts)
    : apts_(apts)
    {
    }

    float MemberParameterSet::getSanctity(size_t index) const
    {
        const auto parameterId = config::parameters::sanctityPrefix + std::to_string(index);
        return apts_.getRawParameterValue(parameterId)->load();
    }

    bool MemberParameterSet::getSummonState(size_t index) const
    {
        const auto parameterId = config::parameters::summonStatePrefix + std::to_string(index);
        return apts_.getRawParameterValue(parameterId)->load();
    }

    float MemberParameterSet::getDistance(size_t index) const
    {
        const auto parameterId = config::parameters::distancePrefix + std::to_string(index);
        return apts_.getRawParameterValue(parameterId)->load();
    }
}


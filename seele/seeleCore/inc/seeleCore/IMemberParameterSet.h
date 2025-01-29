#pragma once

#include <memory>


namespace hidonash
{
    class IMemberParameterSet
    {
    public:
        virtual ~IMemberParameterSet() = default;

        virtual float getSanctity(size_t index) const = 0;

        virtual bool getSummonState(size_t index) const = 0;

        virtual float getDistance(size_t index) const = 0;

        virtual float getGain(size_t index) const = 0;
    };

    using MemberParameterSetPtr = std::unique_ptr<IMemberParameterSet>;
}

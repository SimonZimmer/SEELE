#pragma once

#include <memory>


namespace hidonash
{
    class IMemberParameterSet
    {
    public:
        virtual ~IMemberParameterSet() = default;

        virtual float getPitchRatio(size_t index) const = 0;
    };

    using MemberParameterSetPtr = std::unique_ptr<IMemberParameterSet>;

}


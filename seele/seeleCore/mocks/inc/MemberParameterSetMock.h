#include <seeleCore/IMemberParameterSet.h>


namespace hidonash
{
    class MemberParameterSetMock : public IMemberParameterSet
    {
    public:
        virtual ~MemberParameterSetMock() = default;

        MOCK_METHOD(float, getSanctity, (size_t index), (const));

        MOCK_METHOD(bool, getSummonState, (size_t index), (const));

        MOCK_METHOD(float, getDistance, (size_t index), (const));
    };
}


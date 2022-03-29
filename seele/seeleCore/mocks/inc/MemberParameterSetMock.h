#include <memory>

#include <seeleCore/IMemberParameterSet.h>


namespace hidonash
{
    class MemberParameterSetMock : public IMemberParameterSet
    {
    public:
        virtual ~MemberParameterSetMock() = default;

        MOCK_METHOD(float, getSanctity, (size_t index), (const));

        MOCK_METHOD(float, getSummonState, (size_t index), (const));
    };
}


#include <memory>

#include <seeleCore/IMemberParameterSet.h>


namespace hidonash
{
    class MemberParameterSetMock : public IMemberParameterSet
    {
    public:
        virtual ~MemberParameterSetMock() = default;

        MOCK_METHOD(float, getPitchRatio, (size_t index), (const));
    };
}


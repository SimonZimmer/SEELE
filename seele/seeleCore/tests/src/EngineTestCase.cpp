#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <FactoryMock.h>

namespace hidonash
{
    using namespace testing;

    class UnitTest_Engine : public testing::Test
    {
    public:
       void SetUp() override
       {
           factoryMock_ = std::make_unique<NiceMock<FactoryMock>>();
       }

       void TearDown() override
       {
           factoryMock_ = nullptr;
       }

    protected:
        std::unique_ptr<NiceMock<FactoryMock>> factoryMock_;
    };

}

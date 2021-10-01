#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <AudioProcessorMock.h>
#include <FactoryMock.h>

#include <seeleCore/Engine.h>

namespace sz
{
    using namespace testing;

    class UnitTest_Engine : public testing::Test
    {
    public:
       void SetUp() override
       {
           chopperMock_ = std::make_unique<NiceMock<AudioProcessorMock>>();
           chopperMockPtr_ = chopperMock_.get();
           factoryMock_ = std::make_unique<NiceMock<FactoryMock>>();

           ON_CALL(*factoryMock_, createChopper(_))
               .WillByDefault(Return(ByMove(std::move(chopperMock_))));
       }

       void TearDown() override
       {
           //TODO is this needed?
           chopperMock_ = nullptr;
           chopperMockPtr_ = nullptr;
           factoryMock_ = nullptr;
       }

    protected:
        std::unique_ptr<NiceMock<AudioProcessorMock>> chopperMock_;
        NiceMock<AudioProcessorMock>* chopperMockPtr_;
        std::unique_ptr<NiceMock<FactoryMock>> factoryMock_;
    };

    TEST_F(UnitTest_Engine, DISABLED_create)
    {
        auto&& chopFrequency = std::atomic<float>(100);
        auto&& pitchRatio = std::atomic<float>(0.5);

        EXPECT_CALL(*factoryMock_, createChopper(_));

        Engine(chopFrequency, pitchRatio, *factoryMock_);
    }

    TEST_F(UnitTest_Engine, DISABLED_process)
    {
        auto&& chopFrequency = std::atomic<float>(100);
        auto&& pitchRatio = std::atomic<float>(0.5);
        auto&& buffer = core::AudioBuffer<float>(2, 64);
        auto&& engine = Engine(chopFrequency, pitchRatio, *factoryMock_);
        const auto fakeValue = 0.35f;

        ON_CALL(*chopperMockPtr_, process)
                .WillByDefault(Invoke([&buffer, &fakeValue]()
                {
                    for(auto ch = 0; ch < buffer.getNumChannels(); ++ch)
                        for(auto sa = 0; sa < buffer.getNumSamples(); ++sa)
                            buffer.setSample(ch, sa, fakeValue);
                }));

        EXPECT_CALL(*chopperMockPtr_, process(_));

        engine.process(buffer);

        //TODO implement core::AudioBuffer matcher
        ASSERT_EQ(buffer[0][0], fakeValue);
        ASSERT_EQ(buffer[1][63], fakeValue);
        ASSERT_EQ(buffer[0][32], fakeValue);
    }
}

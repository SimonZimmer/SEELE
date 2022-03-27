#include <cmath>

#include <gtest/gtest.h>

#include <core/AudioBuffer.h>
#include <seeleCore/PitchShifter.h>

#include <FactoryMock.h>
#include <AnalysisMock.h>
#include <SynthesisMock.h>


namespace hidonash
{
    using namespace testing;

    class UnitTest_PitchShifter : public ::testing::Test
    {
    public:
        void SetUp() override
        {
            factoryMock_ = std::make_unique<NiceMock<FactoryMock>>();
            synthesisMock_ = std::make_unique<NiceMock<SynthesisMock>>();
            synthesisMockPtr_ = synthesisMock_.get();

            ON_CALL(*factoryMock_, createSynthesis)
                .WillByDefault(Return(ByMove(std::move(synthesisMock_))));
        }

        void TearDown() override
        {
            factoryMock_ = nullptr;
            synthesisMock_ = nullptr;
        }

        std::unique_ptr<FactoryMock> factoryMock_;
        std::unique_ptr<SynthesisMock> synthesisMock_;
        SynthesisMock* synthesisMockPtr_;
    };

    TEST_F(UnitTest_PitchShifter, construction)
    {
        {
            InSequence sequence;
            EXPECT_CALL(*factoryMock_, createAnalysis(_)).Times(1);
            EXPECT_CALL(*factoryMock_, createSynthesis(_, _)).Times(1);
        }

        PitchShifter(44100, std::move(factoryMock_));
    }

    TEST_F(UnitTest_PitchShifter, process)
    {
        auto&& pitchShifter = PitchShifter(44100, std::move(factoryMock_));
        auto&& buffer = core::AudioBuffer<float>(2, 128);

        //TODO find out why its called 2 times
        EXPECT_CALL(*synthesisMockPtr_, perform(_, _)).Times(2);

        pitchShifter.process(buffer);
    }
}

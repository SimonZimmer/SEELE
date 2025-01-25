#include "seeleCore/Config.h"
#include <cmath>

#include <gtest/gtest.h>

#include <core/AudioBuffer.h>
#include <seeleCore/PitchShifter.h>

#include <FactoryMock.h>
#include <AnalysisMock.h>
#include <SynthesisMock.h>
#include <AudioBufferMock.h>


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

        PitchShifter(44100, *factoryMock_);
    }

    /*

    TEST_F(UnitTest_PitchShifter, process)
    {
        const auto bufferSize = 128;
        auto fakeBuffer = std::array<float, bufferSize>();
        auto channelMock = NiceMock<core::ChannelMock>();

        ON_CALL(channelMock, getNumSamples())
            .WillByDefault(Return(channelSize));
        ON_CALL(channelMock, getDataPointer())
            .WillByDefault(Return(fakeBuffer.data()));
        ON_CALL(channelMock, getSample(_, _))
            .WillByDefault(Return(0.33f));
        ON_CALL(channelMock, setSample(_, _, _))
            .WillByDefault(Return());

        auto pitchShifter = PitchShifter(44100, *factoryMock_);

        EXPECT_CALL(channelMock, getNumSamples()).Times(1);
        EXPECT_CALL(channelMock, getDataPointer()).Times(2 * channelSize);
        EXPECT_CALL(channelMock, getSample(_, _)).Times(2 * channelSize);
        EXPECT_CALL(channelMock, setSample(_, _, _)).Times(channelSize);
        EXPECT_CALL(channelMock, multiply(Matcher<float>(_), Matcher<size_t>(_))).Times(1);
        EXPECT_CALL(*synthesisMockPtr_, perform(_, _)).Times(1);

        pitchShifter.process(channelMock);
    }
    */
}


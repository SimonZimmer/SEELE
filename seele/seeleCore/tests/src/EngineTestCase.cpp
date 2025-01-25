#include <gmock/gmock-actions.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>
#include <seeleCore/Config.h>

#include <FactoryMock.h>
#include <PitchShifterMock.h>
#include <AudioBufferMock.h>
#include <MemberParameterSetMock.h>

#include <seeleCore/Engine.h>


namespace hidonash
{
    using namespace testing;

    class UnitTest_Engine : public testing::Test
    {
    public:
       void SetUp() override
       {
           factoryMock_ = std::make_unique<NiceMock<FactoryMock>>();
           factoryMockPtr_ = factoryMock_.get();
           memberParameterSetMock_ = std::make_unique<NiceMock<MemberParameterSetMock>>();
           pitchShifterMock_ = std::make_unique<NiceMock<PitchShifterMock>>();
           pitchShifterMockPtr_ = pitchShifterMock_.get();
           audioBufferMock_ = std::make_unique<NiceMock<core::AudioBufferMock>>();
           audioBufferMockPtr_ = audioBufferMock_.get();
       }

       void TearDown() override
       {
           factoryMock_ = nullptr;
           memberParameterSetMock_ = nullptr;
           pitchShifterMock_ = nullptr;
           audioBufferMock_ = nullptr;
       }

    protected:
        std::unique_ptr<FactoryMock> factoryMock_;
        FactoryMock* factoryMockPtr_;
        std::unique_ptr<MemberParameterSetMock> memberParameterSetMock_;
        std::unique_ptr<PitchShifterMock> pitchShifterMock_;
        PitchShifterMock* pitchShifterMockPtr_;
        std::unique_ptr<core::AudioBufferMock> audioBufferMock_;
        core::AudioBufferMock* audioBufferMockPtr_;
    };

    TEST_F(UnitTest_Engine, construction)
    {
        EXPECT_CALL(*factoryMockPtr_, createPitchShifter(_, _)).Times(config::constants::numMembers);
        EXPECT_CALL(*factoryMockPtr_, createAudioBuffer(_, _)).Times(config::constants::numMembers);

        Engine(*memberParameterSetMock_, 44100., 64, 1, std::move(factoryMock_));
    }

    TEST_F(UnitTest_Engine, process)
    {
         ON_CALL(*factoryMock_, createPitchShifter(_, _))
             .WillByDefault(Return(ByMove(std::move(pitchShifterMock_))));
         ON_CALL(*factoryMock_, createAudioBuffer(_, _))
             .WillByDefault(Return(ByMove(std::move(audioBufferMock_))));

        auto&& engine = Engine(*memberParameterSetMock_, 44100., 64, 1, std::move(factoryMock_), 1);
        auto inputBufferMock = core::AudioBufferMock();

        EXPECT_CALL(*memberParameterSetMock_, getSummonState(_)).Times(2);
        EXPECT_CALL(inputBufferMock, fill(Eq(0.f))).Times(1);
        EXPECT_CALL(inputBufferMock, multiply(Matcher<float>(_), Matcher<size_t>(_))).Times(1);

        engine.process(inputBufferMock);
    }

    TEST_F(UnitTest_Engine, process_summon_state_true)
    {
         ON_CALL(*factoryMock_, createPitchShifter(_, _))
             .WillByDefault(Return(ByMove(std::move(pitchShifterMock_))));
         ON_CALL(*factoryMock_, createAudioBuffer(_, _))
             .WillByDefault(Return(ByMove(std::move(audioBufferMock_))));
         ON_CALL(*memberParameterSetMock_, getSummonState(_))
             .WillByDefault(Return(true));

        auto&& engine = Engine(*memberParameterSetMock_, 44100., 64, 1, std::move(factoryMock_), 1);
        auto inputBufferMock = core::AudioBufferMock();

        EXPECT_CALL(inputBufferMock, getNumChannels()).Times(1);
        EXPECT_CALL(inputBufferMock, getNumSamples()).Times(3);
        EXPECT_CALL(*audioBufferMockPtr_, setSize(_, _)).Times(1);
        EXPECT_CALL(*audioBufferMockPtr_, copyFrom(_)).Times(1);
        EXPECT_CALL(*memberParameterSetMock_, getSanctity(_)).Times(1);
        EXPECT_CALL(*pitchShifterMockPtr_, setPitchRatio(_)).Times(1);
        EXPECT_CALL(*pitchShifterMockPtr_, process(_)).Times(1);

        engine.process(inputBufferMock);
    }

    TEST_F(UnitTest_Engine, process_summon_state_false)
    {
         ON_CALL(*factoryMock_, createPitchShifter(_, _))
             .WillByDefault(Return(ByMove(std::move(pitchShifterMock_))));
         ON_CALL(*factoryMock_, createAudioBuffer(_, _))
             .WillByDefault(Return(ByMove(std::move(audioBufferMock_))));
         ON_CALL(*memberParameterSetMock_, getSummonState(_))
             .WillByDefault(Return(false));

        auto&& engine = Engine(*memberParameterSetMock_, 44100., 64, 1, std::move(factoryMock_), 1);
        auto inputBufferMock = core::AudioBufferMock();

        EXPECT_CALL(*memberParameterSetMock_, getSummonState(_)).Times(2);
        EXPECT_CALL(*audioBufferMockPtr_, getNumChannels()).Times(0);
        EXPECT_CALL(*audioBufferMockPtr_, getNumSamples()).Times(0);
        EXPECT_CALL(*audioBufferMockPtr_, setSize(_, _)).Times(0);
        EXPECT_CALL(*audioBufferMockPtr_, copyFrom(_)).Times(0);
        EXPECT_CALL(*memberParameterSetMock_, getSanctity(_)).Times(0);
        EXPECT_CALL(*pitchShifterMockPtr_, setPitchRatio(_)).Times(0);
        EXPECT_CALL(*pitchShifterMockPtr_, process(_)).Times(0);

        engine.process(inputBufferMock);
    }
}

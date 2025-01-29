#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <AudioBufferMock.h>
#include <DelayProcessorMock.h>
#include <FactoryMock.h>
#include <GainProcessorMock.h>
#include <MemberParameterSetMock.h>
#include <PitchShifterManagerMock.h>

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
            audioBufferMock_ = std::make_unique<NiceMock<core::AudioBufferMock>>();
            audioBufferMockPtr_ = audioBufferMock_.get();

            ON_CALL(*factoryMockPtr_, createAudioBuffer(_, _))
                .WillByDefault(Return(ByMove(std::move(audioBufferMock_))));
            ON_CALL(*factoryMockPtr_, createPitchShifterManager(_, _, _))
                .WillByDefault(Return(ByMove(std::move(pitchShifterManagerMock_))));
            ON_CALL(*factoryMockPtr_, createDelayProcessor(_, _, _))
                .WillByDefault(Return(ByMove(std::move(delayProcessorMock_))));
            ON_CALL(*factoryMockPtr_, createGainProcessor(_, _))
                .WillByDefault(Return(ByMove(std::move(gainProcessorMock_))));
        }

        void TearDown() override
        {
            factoryMock_ = nullptr;
            memberParameterSetMock_ = nullptr;
            audioBufferMock_ = nullptr;
        }

    protected:
        std::unique_ptr<FactoryMock> factoryMock_;
        FactoryMock* factoryMockPtr_;
        std::unique_ptr<MemberParameterSetMock> memberParameterSetMock_;
        std::unique_ptr<core::AudioBufferMock> audioBufferMock_;
        core::AudioBufferMock* audioBufferMockPtr_;
        std::unique_ptr<PitchShifterManagerMock> pitchShifterManagerMock_;
        PitchShifterManagerMock* pitchShifterManagerMockPtr_;
        std::unique_ptr<DelayProcessorMock> delayProcessorMock_;
        DelayProcessorMock* delayProcessorMockPtr_;
        std::unique_ptr<GainProcessorMock> gainProcessorMock_;
        GainProcessorMock* gainProcessorMockPtr_;
    };
}

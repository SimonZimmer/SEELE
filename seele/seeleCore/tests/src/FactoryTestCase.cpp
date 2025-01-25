#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <core/IAudioBuffer.h>

#include <seeleCore/Factory.h>
#include <seeleCore/IAudioProcessor.h>
#include <seeleCore/IAnalysis.h>
#include <seeleCore/IPitchShifter.h>

#include <AnalysisMock.h>
#include <MemberParameterSetMock.h>
#include <FactoryMock.h>
#include <AudioBufferMock.h>


namespace hidonash
{
    using namespace testing;

    TEST(UnitTest_Factory, createEngine)
    {
        auto pitchRatio = std::atomic<float>(1.f);
        auto memberParameterSetMock = MemberParameterSetMock();
        auto&& engine = Factory().createEngine(memberParameterSetMock, 44100., 64);
        EXPECT_THAT(engine.get(), WhenDynamicCastTo<IAudioProcessor*>(NotNull()));
    }

    TEST(UnitTest_Factory, createAnalysis)
    {
        auto&& analysis = Factory().createAnalysis(2);
        EXPECT_THAT(analysis.get(), WhenDynamicCastTo<IAnalysis*>(NotNull()));
    }

    TEST(UnitTest_Factory, createSynthesis)
    {
        auto&& synthesis = Factory().createSynthesis(2, std::make_unique<AnalysisMock>());
        EXPECT_THAT(synthesis.get(), WhenDynamicCastTo<ISynthesis*>(NotNull()));
    }

    TEST(UnitTest_Factory, createPitchShifter)
    {
        auto&& factoryMock = std::make_unique<NiceMock<FactoryMock>>();
        auto&& pitchShifter = Factory().createPitchShifter(44100., *factoryMock);
        EXPECT_THAT(pitchShifter.get(), WhenDynamicCastTo<IPitchShifter*>(NotNull()));
    }

    TEST(UnitTest_Factory, createAudioBuffer)
    {
        auto&& audioBuffer = Factory().createAudioBuffer(2, 64);
        EXPECT_THAT(audioBuffer.get(), WhenDynamicCastTo<core::IAudioBuffer*>(NotNull()));
    }
}


#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <seeleCore/Factory.h>
#include <seeleCore/IAudioProcessor.h>
#include <seeleCore/IAnalysis.h>

#include <AnalysisMock.h>

namespace hidonash
{
    using namespace testing;

    TEST(UnitTest_Factory, createEngine)
    {
        auto pitchRatio = std::atomic<float>(1.f);
        auto&& engine = Factory().createEngine(pitchRatio, 44100.);
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
}

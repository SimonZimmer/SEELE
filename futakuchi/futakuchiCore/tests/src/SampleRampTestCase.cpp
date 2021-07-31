#include <gtest/gtest.h>

#include <futakuchiCore/SampleRamp.h>

namespace sz
{
    using namespace testing;

    class UnitTest_SampleRamp : public testing::Test {};

    TEST(UnitTest_SampleRamp, startsWithStartingValue)
    {
        auto&& sampleRamp = SampleRamp(0.f, 1.f, 10);
        EXPECT_FLOAT_EQ(sampleRamp.getNextGain(), 0.f);
    }

    TEST(UnitTest_SampleRamp, endsWithEndingValue)
    {
        auto&& sampleRamp = SampleRamp(0.f, 1.f, 10);
        for(auto i = 0; i < 9; ++i)
            sampleRamp.getNextGain();
        EXPECT_FLOAT_EQ(sampleRamp.getNextGain(), 1.f);
    }

    TEST(UnitTest_SampleRamp, rampsContinously_up)
    {
        auto&& sampleRamp = SampleRamp(0.f, 1.f, 10);
        for(auto i = 0; i < 10; ++i)
            EXPECT_LT(sampleRamp.getNextGain(), sampleRamp.getNextGain());
    }

    TEST(UnitTest_SampleRamp, rampsContinously_down)
    {
        auto&& sampleRamp = SampleRamp(1.f, 0.f, 10);
        for(auto i = 0; i < 10; ++i)
            EXPECT_GT(sampleRamp.getNextGain(), sampleRamp.getNextGain());
    }
}

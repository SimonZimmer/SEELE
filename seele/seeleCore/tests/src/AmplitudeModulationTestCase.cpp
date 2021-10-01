#include <gtest/gtest.h>

#include <seeleCore/AmplitudeModulation.h>

namespace sz
{
    using namespace testing;

    class UnitTest_AmplitudeModulation : public testing::Test
    {
    };

    TEST(UnitTest_AmplitudeModulation, chops)
    {
        auto&& chopFrequency = std::atomic<float>(100.f);
        auto&& chopper = AmplitudeModulation(chopFrequency);

        auto buffer = core::AudioBuffer<float>(1, 64);
        for (auto ch = 0; ch < buffer.getNumChannels(); ++ch)
            for (auto sa = 0; sa < buffer.getNumSamples(); ++sa)
                buffer[ch][sa] = 0.5f;

        chopper.process(buffer);

        //TODO find better ways to test this
        for (auto ch = 0; ch < buffer.getNumChannels(); ++ch)
            for (auto sa = 0; sa < buffer.getNumSamples(); ++sa)
                EXPECT_NE(buffer[ch][sa], 0.5f);
    }
}

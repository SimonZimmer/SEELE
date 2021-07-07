#include <gtest/gtest.h>

#include <pupilCore/Chopper.h>

namespace sz
{
    using namespace testing;

    class UnitTest_Chopper : public testing::Test
    {
    };

    TEST(UnitTest_Chopper, chops)
    {
        auto&& chopFrequency = std::atomic<float>(100.f);
        auto&& chopper = Chopper(chopFrequency);

        auto buffer = core::AudioBuffer<float>(1, 1000);
        for (auto ch = 0; ch < buffer.getNumChannels(); ++ch)
            for (auto sa = 0; sa < buffer.getNumSamples(); ++sa)
                buffer[ch][sa] = 0.5f;

        chopper.process(buffer);

        for (auto ch = 0; ch < buffer.getNumChannels(); ++ch)
            for (auto sa = 0; sa < buffer.getNumSamples(); ++sa)
                std::cout << buffer[ch][sa] << std::endl;
    }
}

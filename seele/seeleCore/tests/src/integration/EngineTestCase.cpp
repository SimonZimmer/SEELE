#include <cmath>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <seeleCore/Engine.h>

namespace hidonash
{
    using namespace testing;

    class IntegrationTest_Engine : public testing::Test
    {
    };

    TEST_F(IntegrationTest_Engine, process_lowPitched)
    {
        auto&& pitchRatio = std::atomic<float>(0.5f);
        auto&& fftFrameSize = std::atomic<float>(2);
        auto sampleRate = 44100;
        auto&& engine = Engine(pitchRatio, sampleRate);
        auto&& buffer = core::AudioBuffer<float>(2, 128);
        auto currentAngle = 0.f;

        auto angleDelta = (1000.f / 44100.f) * 2.f * M_PI;
        for(auto c = 0; c < buffer.getNumChannels(); ++c)
            for(auto i = 0; i < buffer.getNumSamples(); ++i)
            {
                buffer.setSample(c, i, std::sin(currentAngle));
                currentAngle += angleDelta;
            }

        for(auto i = 0; i < 20; ++i)
            engine.process(buffer);

        /*for(auto i = 0; i < buffer.getNumSamples(); ++i)
            EXPECT_NEAR(buffer[0][i], referenceHalf_[i], 0.001f);*/
    }
}

#include <cmath>

#include <gtest/gtest.h>

#include <seeleCore/PitchShifter.h>

namespace hidonash
{
    using namespace testing;

    class UnitTest_PitchShifter : public testing::Test
    {
    public:
        void SetUp() override
        {
        }

        void TearDown() override
        {
        }
    };

    TEST_F(UnitTest_PitchShifter, fft)
    {
        auto&& pitchShifter = PitchShifter(44100);
        pitchShifter.setFftFrameSize(64);

        auto&& buffer = core::AudioBuffer<float>(1, 4048);
        auto currentAngle = 0.f;

        auto angleDelta = (10000.f / 44100.f) * 2.f * M_PI;
        for(auto c = 0; c < buffer.getNumChannels(); ++c)
            for(auto i = 0; i < buffer.getNumSamples(); ++i)
            {
                buffer.setSample(c, i, std::sin(currentAngle));
                currentAngle += angleDelta;
            }
        auto&& interleavedBuffer = core::AudioBuffer<float>(1, 2*4048);
        for(auto sa = 0; sa < buffer.getNumSamples(); ++sa)
        {
            interleavedBuffer[0][2 * sa] = buffer[0][sa];
            interleavedBuffer[0][2 * sa + 1] = 0.f;
        }

        pitchShifter.fft(interleavedBuffer.getDataPointer(), 64, false);
    }
}

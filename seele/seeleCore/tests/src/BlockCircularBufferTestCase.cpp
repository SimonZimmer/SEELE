#include <gtest/gtest.h>

#include <core/AudioBuffer.h>

#include <core/BlockCircularBuffer.h>

namespace sz
{
    class AudioBufferTest : public testing::Test
    {
    };

    TEST_F(AudioBufferTest, write)
    {
        /*const auto numChannels = 1;
        const auto numSamples = 64;
        BlockCircularBuffer<float> buffer;

        auto sampleValue = 0;
        for (size_t c = 0; c < numChannels; c++)
            for (size_t i = 0; i < numSamples; ++i)
            {
                buffer.setSample(c, i, incrementedValue);
                EXPECT_EQ(incrementedValue, buffer[c][i]);
            }
            */
    }

}

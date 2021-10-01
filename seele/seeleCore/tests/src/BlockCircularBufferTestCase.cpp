#include <gtest/gtest.h>

#include <core/AudioBuffer.h>

#include <core/BlockCircularBuffer.h>

namespace sz
{
    class AudioBufferTest : public testing::Test
    {
    };

    TEST_F(AudioBufferTest, read_write)
    {
        const auto numChannels = 1;
        const auto numSamples = 64;
        const auto sampleValue = 0.77f;
        BlockCircularBuffer<float> circularBuffer(numSamples);
        auto sourceBuffer = core::AudioBuffer<float>(numChannels, numSamples);
        auto destinationBuffer = core::AudioBuffer<float>(numChannels, numSamples);
        sourceBuffer.fill(sampleValue);
        destinationBuffer.fill(0.f);

        circularBuffer.write(sourceBuffer, 0, numSamples);
        circularBuffer.read(destinationBuffer, 0, numSamples);

        for (size_t ch = 0; ch < numChannels; ch++)
            for (size_t sa = 0; sa < numSamples; ++sa)
                EXPECT_FLOAT_EQ(sourceBuffer[ch][sa], destinationBuffer[ch][sa]);
    }

    TEST_F(AudioBufferTest, read_write_offset)
    {
        const auto numChannels = 1;
        const auto numSamples = 64;
        const auto sampleValue = 0.77f;
        const auto samplePosition = 54;
        const auto offset = 17;
        BlockCircularBuffer<float> circularBuffer(numSamples);
        auto sourceBuffer = core::AudioBuffer<float>(numChannels, numSamples);
        auto destinationBuffer = core::AudioBuffer<float>(numChannels, numSamples);
        sourceBuffer.fill(0.f);
        sourceBuffer[0][samplePosition] = sampleValue;
        destinationBuffer.fill(0.f);

        circularBuffer.write(sourceBuffer, offset, numSamples);
        circularBuffer.read(destinationBuffer, offset, numSamples);

        for (size_t ch = 0; ch < numChannels; ch++)
            for (size_t sa = 0; sa < numSamples; ++sa)
                EXPECT_FLOAT_EQ(sourceBuffer[ch][sa], destinationBuffer[ch][sa]);
    }

    TEST_F(AudioBufferTest, read_write_numSamples)
    {
        const auto numChannels = 1;
        const auto numSamples = 64;
        const auto sampleValue = 0.77f;
        const auto writeLength = numSamples / 2;
        BlockCircularBuffer<float> circularBuffer(numSamples);
        auto sourceBuffer = core::AudioBuffer<float>(numChannels, numSamples);
        auto destinationBuffer = core::AudioBuffer<float>(numChannels, numSamples);
        sourceBuffer.fill(sampleValue);
        destinationBuffer.fill(0.f);

        circularBuffer.write(sourceBuffer, 0, writeLength);
        circularBuffer.read(destinationBuffer, 0, writeLength);

        for (size_t ch = 0; ch < numChannels; ch++)
            for (size_t sa = 0; sa < numSamples; ++sa)
                EXPECT_FLOAT_EQ(sourceBuffer[ch][sa], destinationBuffer[ch][sa]);
    }

}

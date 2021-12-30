#include <gtest/gtest.h>

#include <core/AudioBuffer.h>

#include <core/BlockCircularBuffer.h>

namespace sz
{
    class BlockCircularBufferTest : public  testing::Test
    {
    protected:
        const int numChannels_ = 1;
        const int numSamples_ = 64;
        BlockCircularBuffer<float> circularBuffer_{64};
    };

    TEST_F(BlockCircularBufferTest, read_write)
    {
        const auto sampleValue = 0.77f;
        auto sourceBuffer = core::AudioBuffer<float>(numChannels_, numSamples_);
        auto destinationBuffer = core::AudioBuffer<float>(numChannels_, numSamples_);
        sourceBuffer.fill(sampleValue);
        destinationBuffer.fill(0.f);

        circularBuffer_.write(sourceBuffer, 0, numSamples_);
        circularBuffer_.read(destinationBuffer, 0, numSamples_);

        for (size_t ch = 0; ch < numChannels_; ch++)
            for (size_t sa = 0; sa < numSamples_; ++sa)
                EXPECT_FLOAT_EQ(sourceBuffer[ch][sa], destinationBuffer[ch][sa]);
    }

    TEST_F(BlockCircularBufferTest, reset)
    {
        auto sourceBuffer = core::AudioBuffer<float>(numChannels_, numSamples_);
        sourceBuffer.fill(0.99f);

        circularBuffer_.reset();

        for (size_t ch = 0; ch < numChannels_; ch++)
            for (size_t sa = 0; sa < numSamples_; ++sa)
                EXPECT_FLOAT_EQ(0.f, circularBuffer_.getDataPointer()[ch * sa]);
    }

    TEST_F(BlockCircularBufferTest, overlapWrite)
    {
        const auto numSamples = numSamples_ + numSamples_ / 2;
        const auto sampleValueA = 0.333f;
        const auto sampleValueB = 0.222f;
        auto buffer = core::AudioBuffer<float>(numChannels_, numSamples);
        buffer.fill(sampleValueA);
        buffer[0][numSamples_] = sampleValueB;
        circularBuffer_.overlapWrite(buffer, numSamples);

        EXPECT_FLOAT_EQ(circularBuffer_.getDataPointer()[0], sampleValueB);
    }
}

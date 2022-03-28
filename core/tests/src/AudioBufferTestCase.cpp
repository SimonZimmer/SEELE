#include <gtest/gtest.h>

#include <core/AudioBuffer.h>

namespace
{
    using namespace testing;

    struct Parameter
    {
        size_t noChannels;
        size_t noSamples;
    };

    std::vector<Parameter> createTestParameters()
    {
        std::vector<Parameter> params;
        for (size_t noChannels : { 1, 2 })
            for (size_t noSamples : { 32, 64, 128, 256, 512 })
                params.push_back(Parameter({ noChannels, noSamples }));
        return params;
    }

    std::string paramToString(TestParamInfo<Parameter> testParam)
    {
        return
                std::to_string(testParam.param.noChannels) + "_" +
                std::to_string(testParam.param.noSamples);
    }
}

namespace hidonash
{
    class AudioBufferTest : public TestWithParam<Parameter> {};

    INSTANTIATE_TEST_SUITE_P(UnitTest_AudioBuffer,
                            AudioBufferTest,
                            ::testing::ValuesIn(createTestParameters()),
                            paramToString);

    TEST_P(AudioBufferTest, construction)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        const core::AudioBuffer buffer(noChannels, noSamples);

        EXPECT_EQ(noChannels, buffer.getNumChannels());
        EXPECT_EQ(noSamples, buffer.getNumSamples());

        for (size_t c = 0; c < noChannels; c++)
            for (size_t i = 0; i < noSamples; ++i)
                EXPECT_EQ(0.f, buffer.getSample(c, i));
    }

    TEST(AudioBufferTest, construction_empty)
    {
        const core::AudioBuffer buffer(0, 0);

        EXPECT_EQ(0, buffer.getNumChannels());
        EXPECT_EQ(0, buffer.getNumSamples());
    }

    TEST_P(AudioBufferTest, setSize)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer buffer(noChannels, noSamples);
        buffer.setSize(3, 77);

        EXPECT_EQ(buffer.getNumChannels(), 3);
        EXPECT_EQ(buffer.getNumSamples(), 77);
    }

    TEST_P(AudioBufferTest, setSample)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer buffer(noChannels, noSamples);

        EXPECT_EQ(noChannels, buffer.getNumChannels());
        EXPECT_EQ(noSamples, buffer.getNumSamples());

        auto sampleValue = 0;
        for (size_t c = 0; c < noChannels; c++)
            for (size_t i = 0; i < noSamples; ++i)
            {
                const auto incrementedValue = sampleValue++;
                buffer.setSample(c, i, incrementedValue);
                EXPECT_EQ(incrementedValue, buffer.getSample(c, i));
            }
    }

    TEST_P(AudioBufferTest, fill)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer buffer(noChannels, noSamples);

        buffer.fill(0.33f);

        for (size_t c = 0; c < noChannels; c++)
            for (size_t i = 0; i < noSamples; ++i)
                EXPECT_EQ(0.33f, buffer.getSample(c, i));
    }

    TEST_P(AudioBufferTest, copyFrom)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer buffer(noChannels, noSamples);
        buffer.fill(0.44f);

        core::AudioBuffer otherBuffer(noChannels, noSamples);
        otherBuffer.copyFrom(buffer);

        for (size_t c = 0; c < noChannels; c++)
            for (size_t i = 0; i < noSamples; ++i)
                EXPECT_EQ(0.44f, otherBuffer.getSample(c, i));
    }

    TEST_P(AudioBufferTest, copy)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer bufferA(noChannels, noSamples);
        core::AudioBuffer bufferB(noChannels, noSamples);
        bufferA.copy(bufferB, 0, 0, noSamples);

        for (size_t c = 0; c < noChannels; c++)
            for (size_t i = 0; i < noSamples; ++i)
                EXPECT_EQ(bufferA.getSample(c, i), bufferB.getSample(c, i));
    }

    TEST_P(AudioBufferTest, add)
    {
        const auto noChannels = 1;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer bufferA(noChannels, noSamples);
        bufferA.fill(0.333f);
        core::AudioBuffer bufferB(noChannels, noSamples);
        bufferB.fill(0.222f);
        bufferB.setSample(0, 0, 0.111f);
        bufferA.add(bufferB, noSamples / 2, 1, 1);

        EXPECT_EQ(bufferA.getSample(0, 0), 0.333f);
        EXPECT_EQ(bufferA.getSample(0, 1), 0.555f);
        EXPECT_EQ(bufferA.getSample(0, noSamples / 2 + 1), 0.333f);
    }

    TEST_P(AudioBufferTest, multiply_value)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer buffer(noChannels, noSamples);
        buffer.fill(0.5f);
        buffer.multiply(0.222f, noSamples);

        for (size_t ch = 0; ch < noChannels; ch++)
            for (size_t sa = 0; sa < noSamples; ++sa)
                EXPECT_FLOAT_EQ(buffer.getSample(ch, sa), 0.111f);
    }

    TEST_P(AudioBufferTest, multiply_vector)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer buffer(noChannels, noSamples);
        buffer.fill(0.5f);
        std::vector<float> vector(noSamples);
        std::fill(vector.begin(), vector.end(), 0.222f);
        buffer.multiply(vector, noSamples);

        for (size_t ch = 0; ch < noChannels; ch++)
            for (size_t sa = 0; sa < noSamples; ++sa)
                EXPECT_FLOAT_EQ(buffer.getSample(ch, sa), 0.111f);
    }
}

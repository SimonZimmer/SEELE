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

namespace sz
{

    class AudioBufferTest : public TestWithParam<Parameter>
    {
    };

    INSTANTIATE_TEST_SUITE_P(UnitTest_AudioBuffer,
                            AudioBufferTest,
                            ::testing::ValuesIn(createTestParameters()),
                            paramToString);

    TEST_P(AudioBufferTest, construction)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        const core::AudioBuffer<float> buffer(noChannels, noSamples);

        EXPECT_EQ(noChannels, buffer.getNumChannels());
        EXPECT_EQ(noSamples, buffer.getNumSamples());

        for (size_t c = 0; c < noChannels; c++)
            for (size_t i = 0; i < noSamples; ++i)
                EXPECT_EQ(0.f, buffer[c][i]);
    }

    TEST(AudioBufferTest, construction_empty)
    {
        const core::AudioBuffer<float> buffer(0, 0);

        EXPECT_EQ(0, buffer.getNumChannels());
        EXPECT_EQ(0, buffer.getNumSamples());
    }

    TEST_P(AudioBufferTest, setSample)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer<float> buffer(noChannels, noSamples);

        EXPECT_EQ(noChannels, buffer.getNumChannels());
        EXPECT_EQ(noSamples, buffer.getNumSamples());

        auto sampleValue = 0;
        for (size_t c = 0; c < noChannels; c++)
            for (size_t i = 0; i < noSamples; ++i)
            {
                const auto incrementedValue = sampleValue++;
                buffer.setSample(c, i, incrementedValue);
                EXPECT_EQ(incrementedValue, buffer[c][i]);
            }
    }

    TEST_P(AudioBufferTest, fill)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer<float> buffer(noChannels, noSamples);

        buffer.fill(0.33f);

        for (size_t c = 0; c < noChannels; c++)
            for (size_t i = 0; i < noSamples; ++i)
                EXPECT_EQ(0.33f, buffer[c][i]);
    }

    //TODO write custom AudioBuffer matcher
    TEST_P(AudioBufferTest, copyFrom)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer<float> buffer(noChannels, noSamples);
        buffer.fill(0.44f);

        core::AudioBuffer<float> otherBuffer(noChannels, noSamples);
        otherBuffer.copyFrom(buffer);

        for (size_t c = 0; c < noChannels; c++)
            for (size_t i = 0; i < noSamples; ++i)
                EXPECT_EQ(0.44f, otherBuffer[c][i]);
    }

    TEST_P(AudioBufferTest, setSize)
    {
        const auto noChannels = GetParam().noChannels;
        const auto noSamples = GetParam().noSamples;

        core::AudioBuffer<float> buffer(noChannels, noSamples);
        buffer.setSize(3, 77);

        EXPECT_EQ(buffer.getNumChannels(), 3);
        EXPECT_EQ(buffer.getNumSamples(), 77);
    }
}

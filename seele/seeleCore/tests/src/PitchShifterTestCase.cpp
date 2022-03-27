#include <cmath>

#include <gtest/gtest.h>

#include <core/AudioBuffer.h>
#include <seeleCore/PitchShifter.h>

#include <FactoryMock.h>
#include <AnalysisMock.h>
#include <SynthesisMock.h>


namespace hidonash
{
    using namespace testing;

    class UnitTest_PitchShifter : public ::testing::Test
    {
    public:
        void SetUp() override
        {
            factoryMock_ = std::make_unique<NiceMock<FactoryMock>>();
            analysisMock_ = std::make_unique<NiceMock<AnalysisMock>>();
            analysisMockPtr_ = analysisMock_.get();
            synthesisMock_ = std::make_unique<NiceMock<SynthesisMock>>();
            synthesisMockPtr_ = synthesisMock_.get();

            ON_CALL(*factoryMock_, createAnalysis)
                .WillByDefault(Return(ByMove(std::move(analysisMock_))));
            ON_CALL(*factoryMock_, createSynthesis)
                .WillByDefault(Return(ByMove(std::move(synthesisMock_))));
        }

        void TearDown() override
        {
            factoryMock_ = nullptr;
            analysisMock_ = nullptr;
            synthesisMock_ = nullptr;
        }

        std::unique_ptr<FactoryMock> factoryMock_;
        std::unique_ptr<AnalysisMock> analysisMock_;
        AnalysisMock* analysisMockPtr_;
        std::unique_ptr<SynthesisMock> synthesisMock_;
        SynthesisMock* synthesisMockPtr_;

        std::array<float, 8192> fakeAnalysisBuffer_{ 8192 };
        std::array<float, 8192> fakeSynthesisBuffer;
    };

    TEST_F(UnitTest_PitchShifter, construction)
    {
        EXPECT_CALL(*factoryMock_, createAnalysis(_)).Times(1);
        EXPECT_CALL(*factoryMock_, createSynthesis(_)).Times(1);

        PitchShifter(44100, std::move(factoryMock_));
    }

    TEST_F(UnitTest_PitchShifter, process)
    {
        ON_CALL(*analysisMockPtr_, getMagnitudeBuffer)
            .WillByDefault(Return(fakeAnalysisBuffer_));
        ON_CALL(*analysisMockPtr_, getFrequencyBuffer)
                .WillByDefault(Return(fakeAnalysisBuffer_));
        ON_CALL(*synthesisMockPtr_, getMagnitudeBuffer)
                .WillByDefault(ReturnRef(fakeSynthesisBuffer));
        ON_CALL(*synthesisMockPtr_, getFrequencyBuffer)
                .WillByDefault(ReturnRef(fakeSynthesisBuffer));

        auto&& pitchShifter = PitchShifter(44100, std::move(factoryMock_));
        auto&& buffer = core::AudioBuffer<float>(2, 128);

        //TODO find out why its called 2 times
        EXPECT_CALL(*analysisMockPtr_, perform(_)).Times(2);
        EXPECT_CALL(*analysisMockPtr_, getMagnitudeBuffer()).Times(2);
        EXPECT_CALL(*analysisMockPtr_, getFrequencyBuffer()).Times(2);
        EXPECT_CALL(*synthesisMockPtr_, reset()).Times(2);
        EXPECT_CALL(*synthesisMockPtr_, getMagnitudeBuffer()).Times(2);
        EXPECT_CALL(*synthesisMockPtr_, getFrequencyBuffer()).Times(2);
        EXPECT_CALL(*synthesisMockPtr_, perform(_)).Times(2);

        pitchShifter.process(buffer);
    }
}

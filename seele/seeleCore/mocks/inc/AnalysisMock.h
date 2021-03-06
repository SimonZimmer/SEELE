#pragma once

#include <gmock/gmock.h>

#include <seeleCore/IAnalysis.h>


namespace hidonash
{
    class AnalysisMock : public IAnalysis
    {
    public:
        ~AnalysisMock() override = default;

        MOCK_CONST_METHOD0(getMagnitudeBuffer, std::array<float, config::constants::analysisSize>());

        MOCK_CONST_METHOD0(getFrequencyBuffer, std::array<float, config::constants::analysisSize>());

        MOCK_METHOD(void, perform, (juce::dsp::Complex<float>*));
    };
}

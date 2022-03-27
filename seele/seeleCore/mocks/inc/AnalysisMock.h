#pragma once

#include <gmock/gmock.h>

#include <seeleCore/IAnalysis.h>


namespace hidonash
{
    class AnalysisMock : public IAnalysis
    {
    public:
        ~AnalysisMock() override = default;

        MOCK_METHOD(std::vector<float>&, getMagnitudeBuffer, ());

        MOCK_METHOD(std::vector<float>&, getFrequencyBuffer, ());

        MOCK_METHOD(void, perform, (juce::dsp::Complex<float>*));
    };
}

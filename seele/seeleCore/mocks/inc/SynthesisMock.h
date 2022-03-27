#pragma once

#include <gmock/gmock.h>

#include <seeleCore/Config.h>
#include <seeleCore/ISynthesis.h>


namespace hidonash
{
    class SynthesisMock : public ISynthesis
    {
    public:
        ~SynthesisMock() override = default;

        MOCK_METHOD(void, reset, ());

        MOCK_METHOD(void, perform, (juce::dsp::Complex<float>* fftWorkspace));

        MOCK_METHOD0(getMagnitudeBuffer, std::array<float, 8192>&());

        MOCK_METHOD0(getFrequencyBuffer, std::array<float, 8192>&());
    };
}

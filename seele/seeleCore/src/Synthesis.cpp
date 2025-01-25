#include <cmath>

#include "Synthesis.h"
#include "Config.h"


namespace hidonash
{
    Synthesis::Synthesis(int freqPerBin, AnalysisPtr analysis)
    : analysis_(std::move(analysis))
    , freqPerBin_(freqPerBin)
    , sumPhase_{}
    , frequencyBuffer_{}
    , magnitudeBuffer_{}
    {
    }

    void Synthesis::perform(juce::dsp::Complex<float>* fftWorkspace, float pitchFactor)
    {
        analysis_->perform(fftWorkspace);
        
        std::fill(magnitudeBuffer_.begin(), magnitudeBuffer_.end(), 0.0f);
        std::fill(frequencyBuffer_.begin(), frequencyBuffer_.end(), 0.0f);
        
        auto&& analysisMagnitudeBuffer = analysis_->getMagnitudeBuffer();
        auto&& analysisFrequencyBuffer = analysis_->getFrequencyBuffer();
        
        const int halfFrameSize = config::constants::fftFrameSize / 2;
        
        for (int sa = 0; sa <= halfFrameSize; sa++)
        {
            float targetIndex = static_cast<float>(sa) * pitchFactor;
            int index = static_cast<int>(std::floor(targetIndex));
            
            if (index >= 0 && index <= halfFrameSize)
            {
                float weight = targetIndex - index;
                magnitudeBuffer_[index] += analysisMagnitudeBuffer[sa] * (1.0f - weight);
                
                if (index + 1 <= halfFrameSize)
                    magnitudeBuffer_[index + 1] += analysisMagnitudeBuffer[sa] * weight;
                
                frequencyBuffer_[index] = analysisFrequencyBuffer[sa] * pitchFactor;
            }
        }
        
        for (int sa = 0; sa <= config::constants::fftFrameSize; sa++)
        {
            const auto magnitude = magnitudeBuffer_[sa];
            auto phase = frequencyBuffer_[sa];
            
            auto phaseDifference = phase - static_cast<double>(sa) * freqPerBin_;
            phaseDifference /= freqPerBin_;
            phaseDifference = 2. * M_PI * phaseDifference / config::constants::oversamplingFactor;
            phaseDifference += static_cast<double>(sa) * config::constants::expectedPhaseDifference;
            
            sumPhase_[sa] += phaseDifference;
            phase = sumPhase_[sa];
            
            fftWorkspace[sa].real(magnitude * std::cos(phase));
            fftWorkspace[sa].imag(magnitude * std::sin(phase));
        }
    }
}


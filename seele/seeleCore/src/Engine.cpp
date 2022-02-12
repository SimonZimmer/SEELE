#include "Engine.h"

namespace hidonash
{
    Engine::Engine(std::atomic<float>& pitchRatio, std::atomic<float>& fftFrameSize, double sampleRate,
                   const IFactory& /*factory*/)
    : pitchShifter(sampleRate)
    , pitchRatio_(pitchRatio)
    , fftFrameSize_(fftFrameSize)
    {
        pitchShifter.setPitchRatio(pitchRatio);
        pitchShifter.setFftFrameSize(fftFrameSize);
    }

    // TODO introduce prepareToPlay

    void Engine::process(core::AudioBuffer<float>& inputBuffer)
    {
        pitchShifter.setPitchRatio(pitchRatio_);
        pitchShifter.setFftFrameSize(fftFrameSize_);
        pitchShifter.process(inputBuffer);
    }
}

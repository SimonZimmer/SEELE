#include "DelayProcessor.h"


namespace hidonash
{
    DelayProcessor::DelayProcessor(size_t maxDelaySamples, float delaySamples, double sampleRate)
    : circularBuffer_(maxDelaySamples + 1, 0.0f)
    , writeIndex_(0)
    , readPosition_(0.0f)
    , currentDelay_(delaySamples)
    , targetDelay_(delaySamples)
    , sampleRate_(sampleRate)
    {
        // Ensure initial read position is correct
        readPosition_ = fmod(writeIndex_ - currentDelay_ + circularBuffer_.size(), 
                           static_cast<float>(circularBuffer_.size()));
    }

    void DelayProcessor::process(core::IAudioBuffer::IChannel& input)
    {
        const auto bufferSize = input.size();
        const float interpolationRate = 0.001f; // Adjust this to control smoothing speed
        
        for (auto sa = 0; sa < bufferSize; ++sa)
        {
            // Write input to buffer
            circularBuffer_[writeIndex_] = input[sa];
            
            // Smoothly interpolate current delay towards target
            if (currentDelay_ != targetDelay_)
            {
                float delta = targetDelay_ - currentDelay_;
                currentDelay_ += delta * interpolationRate;
                
                // Snap to target if we're very close
                if (std::abs(delta) < 0.01f)
                    currentDelay_ = targetDelay_;
            }
            
            // Calculate the read position
            readPosition_ = writeIndex_ - currentDelay_;
            
            // Wrap read position if needed
            while (readPosition_ < 0)
                readPosition_ += circularBuffer_.size();
            while (readPosition_ >= circularBuffer_.size())
                readPosition_ -= circularBuffer_.size();
            
            // Get integer and fractional parts for interpolation
            int readIndex = static_cast<int>(readPosition_);
            float fraction = readPosition_ - readIndex;
            
            // Get the two samples we'll interpolate between
            float sample1 = circularBuffer_[readIndex];
            float sample2 = circularBuffer_[(readIndex + 1) % circularBuffer_.size()];
            
            // Linear interpolation between the samples
            input[sa] = sample1 + fraction * (sample2 - sample1);
            
            // Increment write index
            writeIndex_++;
            writeIndex_ %= circularBuffer_.size();
        }
    }

    void DelayProcessor::setDelayInSamples(int delayInSamples)
    {
        float newDelay = std::max(0.0f, std::min(static_cast<float>(delayInSamples),
                                                 static_cast<float>(circularBuffer_.size() - 1)));
        targetDelay_ = newDelay;
    }
}


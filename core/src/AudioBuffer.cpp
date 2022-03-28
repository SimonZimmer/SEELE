#include "AudioBuffer.h"


namespace hidonash::core
{
    AudioBuffer::AudioBuffer(int numChannels, int numSamples)
    : numChannels_(numChannels)
    , numSamples_(numSamples)
    , memoryBlock_(numChannels, numSamples)
    , data_(memoryBlock_.getData())
    {
    }

    AudioBuffer::AudioBuffer(float** dataToReferTo, int numChannels, int numSamples)
    : numChannels_(numChannels)
    , numSamples_(numSamples)
    , data_(dataToReferTo)
    {
    }

    AudioBuffer::~AudioBuffer() = default;

    float AudioBuffer::getSample(int channel, int sample) const
    {
        return *(data_[channel] + sample);
    }

    void AudioBuffer::setSample(int destChannel, int destSample, float newValue)
    {
        *(data_[destChannel] + destSample) = newValue;
    }

    int AudioBuffer::getNumChannels() const
    {
        return numChannels_;
    }

    int AudioBuffer::getNumSamples() const
    {
        return numSamples_;
    }

    float* AudioBuffer::getDataPointer() const
    {
        return *data_;
    }

    void AudioBuffer::fill(float value)
    {
        for(auto ch = 0; ch < numChannels_; ++ch)
            for(auto sa = 0; sa < numSamples_; ++sa)
                setSample(ch, sa, value);
    }

    void AudioBuffer::copyFrom(const IAudioBuffer& other)
    {
        if (other.getNumChannels() == numChannels_ && other.getNumSamples() == numSamples_)
            memcpy(data_[0], other.getDataPointer(), numChannels_ * numSamples_ * sizeof(float));
        else
        {
            const auto channelsToCopy = std::min(other.getNumChannels(), numChannels_);
            const auto samplesToCopy = std::min(numSamples_, other.getNumSamples());

            for (auto c = size_t{ 0 }; c < channelsToCopy; ++c)
                memcpy(data_[c], other.getDataPointer(), samplesToCopy * sizeof(float));
        }
    }

    void AudioBuffer::copy(const IAudioBuffer& from, size_t fromOffset, size_t internalOffset, size_t copyLength)
    {
        memcpy(getDataPointer() + internalOffset, from.getDataPointer() + fromOffset, copyLength * sizeof(float));
    }

    void AudioBuffer::add(const IAudioBuffer& from, const size_t addLength, const size_t fromOffset, const size_t internalOffset)
    {
        const auto numChannels = std::min(numChannels_, from.getNumChannels());

        for(size_t c = 0; c < numChannels; ++c)
            for (size_t i = 0 ; i < addLength; ++i)
                data_[c][i + internalOffset] += from.getSample(c, i + fromOffset);
    }

    void AudioBuffer::multiply(float value, size_t multiplyLength)
    {
        for(size_t c = 0; c < getNumChannels(); ++c)
            for (size_t i = 0 ; i < multiplyLength; ++i)
                data_[c][i] *= value;
    }

    void AudioBuffer::multiply(const std::vector<float>& from, size_t multiplyLength)
    {
        const auto numChannels = std::min(numChannels_, static_cast<int>(from.size()));

        for(size_t c = 0; c < numChannels; ++c)
            for (size_t i = 0 ; i < multiplyLength; ++i)
                data_[c][i] *= from[i];
    }

    void AudioBuffer::setSize(size_t channelCount, size_t sampleCountPerChannel)
    {
        memoryBlock_ = MemoryBlock(channelCount, sampleCountPerChannel);
        data_ = memoryBlock_.getData();
        numChannels_ = channelCount;
        numSamples_ = sampleCountPerChannel;
    }
}


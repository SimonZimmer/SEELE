#pragma once

#include <vector>
#include <memory>


namespace hidonash::core
{
    class IAudioBuffer
    {
    public:
        virtual ~IAudioBuffer() = default;

        virtual float getSample(int channel, int sample) const = 0;

        virtual void setSample(int destChannel, int destSample, float newValue) = 0;

        virtual int getNumChannels() const = 0;

        virtual int getNumSamples() const = 0;

        virtual float* getDataPointer() const = 0;

        virtual void fill(float value) = 0;

        virtual void copyFrom(const IAudioBuffer& other) = 0;

        virtual void copy(const IAudioBuffer& from, size_t fromOffset, size_t internalOffset, size_t copyLength) = 0;

        virtual void add(const IAudioBuffer& from, const size_t addLength, const size_t fromOffset = 0, const size_t internalOffset = 0) = 0;

        virtual void multiply(float value, size_t multiplyLength) = 0;

        virtual void multiply(const std::vector<float>& from, size_t multiplyLength) = 0;

        virtual void setSize(size_t channelCount, size_t sampleCountPerChannel) = 0;
    };
}


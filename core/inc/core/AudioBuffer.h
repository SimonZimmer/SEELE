#pragma once

#include <vector>

namespace sz::core
{
    template <typename T>
    class AudioBuffer
    {
    public:
        AudioBuffer(int numChannels, int numSamples)
        : numChannels_(numChannels)
        , numSamples_(numSamples)
        , memoryBlock_(numChannels, numSamples)
        , data_(memoryBlock_.getData())
        {
        }

        AudioBuffer(T** dataToReferTo, int numChannels, int numSamples)
        : numChannels_(numChannels)
        , numSamples_(numSamples)
        , data_(dataToReferTo)
        {
        }

        T*& operator[](size_t sample) const { return data_[sample]; };

        void setSample(int destChannel, int destSample, T newValue)
        {
            *(data_[destChannel] + destSample) = newValue;
        }

        int getNumChannels() const
        {
            return numChannels_;
        }

        int getNumSamples() const
        {
            return numSamples_;
        }

        float* getDataPointer() const
        {
            return *data_;
        }

        void fill(float value)
        {
            for(auto ch = 0; ch < numChannels_; ++ch)
                for(auto sa = 0; sa < numSamples_; ++sa)
                    setSample(ch, sa, value);
        }

        void copyFrom(const AudioBuffer<T>& other)
        {
            if (other.getNumChannels() == numChannels_ && other.getNumSamples() == numSamples_)
                memcpy(data_[0], other.data_[0], numChannels_ * numSamples_ * sizeof(T));
            else
            {
                const auto channelsToCopy = std::min(other.getNumChannels(), numChannels_);
                const auto samplesToCopy = std::min(numSamples_, other.getNumSamples());

                for (auto c = size_t{ 0 }; c < channelsToCopy; ++c)
                    memcpy(data_[c], other.data_[c], samplesToCopy * sizeof(T));
            }
        }

        void setSize(size_t channelCount, size_t sampleCountPerChannel)
        {
            memoryBlock_ = MemoryBlock(channelCount, sampleCountPerChannel);
            data_ = memoryBlock_.getData();
            numChannels_ = channelCount;
            numSamples_ = sampleCountPerChannel;
        }

    private:
            class MemoryBlock
            {
            public:
                MemoryBlock() = default;

                MemoryBlock(int numChannels, int numSamples)
                : data_(numChannels * numSamples, T(0))
                , channels_(numChannels, nullptr)
                {
                    if (numSamples == 0 && numChannels > 0)
                        return;

                    for (size_t c = 0; c < channels_.size(); ++c)
                        channels_[c] = &data_[c * numSamples];
                }

                T** getData()
                {
                    if (channels_.empty())
                        return nullptr;

                    return &channels_[0];
                }

            private:
                std::vector<T> data_;
                std::vector<T*> channels_;
            };

        private:
            int numChannels_ = 0;
            int numSamples_ = 0;

            MemoryBlock memoryBlock_;
            T** data_;
        };
}

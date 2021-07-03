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

        T getSample(int channelIndex, int sampleIndex) const noexcept
        {
            return *(data_[channelIndex] + sampleIndex);
        }

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

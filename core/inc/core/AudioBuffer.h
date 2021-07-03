#pragma once

#include <vector>

namespace sz::core
{
    template <typename T>
    class AudioBuffer
    {
    public:
        AudioBuffer(int numChannels, int numSamples)
        : data_(numChannels * numSamples, T(0))
        , numChannels_(numChannels)
        , numSamples_(numSamples)
        , channels_(numChannels, nullptr)
        {
            if (numSamples == 0 && numChannels > 0)
                return;

            for (int c = 0; c < channels_.size(); ++c)
                channels_[c] = &data_[c * numSamples];
        }

        T getSample(int channelIndex, int sampleIndex) const noexcept
        {
            return *(channels_[channelIndex] + sampleIndex);
        }

        void setSample(int destChannel, int destSample, T newValue)
        {
            *(channels_[destChannel] + destSample) = newValue;
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
            int numChannels_ = 0;
            int numSamples_ = 0;
            std::vector<T> data_;
            std::vector<T*> channels_;
        };
}

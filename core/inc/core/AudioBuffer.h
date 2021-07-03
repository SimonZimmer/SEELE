#pragma once

namespace sz::core
{
    template <typename T>
    class AudioBuffer
    {
    public:
        AudioBuffer(size_t numChannels, size_t numSamples)
        : data_(numChannels * numSamples, T(0))
        , numChannels_(numChannels)
        , numSamples_(numSamples)
        , channels_(numChannels, nullptr)
        {
            if (numSamples == 0 && numChannels > 0)
                return;

            for (size_t c = 0; c < channels_.size(); ++c)
                channels_[c] = &data_[c * numSamples];
        }

        T getSample(size_t channelIndex, size_t sampleIndex) const noexcept
        {
            return *(channels_[channelIndex] + sampleIndex);
        }

        void setSample (size_t destChannel, size_t destSample, T newValue) const noexcept
        {
            *(channels_[destChannel] + destSample) = newValue;
        }

        size_t getNumChannels() const
        {
            return numChannels_;
        }

        size_t getNumSamples() const
        {
            return numSamples_;
        }

        private:
            size_t numChannels_ = 0;
            size_t numSamples_ = 0;
            std::vector<T> data_;
            std::vector<T*> channels_;
        };
}

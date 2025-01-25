#pragma once

#include "core/IAudioBuffer.h"


namespace hidonash::core
{
    class AudioBuffer : public IAudioBuffer
    {
    public:
        class Channel
        {
        public:
            const float& operator[](size_t sample) const;

            float& operator[](size_t sample);

            size_t size() const;

            void fill(float value);

            void applyGain(float gain);

        private:
            friend AudioBuffer;

            explicit Channel(float* buffer, size_t size);

        private:
            float* buffer_;
            size_t size_;
        };

    public:
        AudioBuffer(int numChannels, int numSamples);

        AudioBuffer(float* const* dataToReferTo, int numChannels, int numSamples);

        ~AudioBuffer() override = default;

        float getSample(int channel, int sample) const override;

        void setSample(int destChannel, int destSample, float newValue) override;

        int getNumChannels() const override;

        int getNumSamples() const override;

        float* getDataPointer() const override;

        const AudioBuffer::Channel getChannel(size_t channel) const;

        AudioBuffer::Channel getChannel(size_t channel);

        void fill(float value) override;

        void copyFrom(const IAudioBuffer& other) override;

        void copy(const IAudioBuffer& from, size_t fromOffset, size_t internalOffset, size_t copyLength) override;

        void add(const IAudioBuffer& from, const size_t addLength, const size_t fromOffset = 0, const size_t internalOffset = 0) override;

        void multiply(float value, size_t multiplyLength) override;

        void multiply(const std::vector<float>& from, size_t multiplyLength) override;

        void setSize(size_t channelCount, size_t sampleCountPerChannel) override;

    private:
        class MemoryBlock
        {
        public:
            MemoryBlock() = default;

            MemoryBlock(int numChannels, int numSamples)
            : data_(numChannels * numSamples, float(0))
            , channels_(numChannels, nullptr)
            {
                if (numSamples == 0 && numChannels > 0)
                    return;

                for (size_t c = 0; c < channels_.size(); ++c)
                    channels_[c] = &data_[c * numSamples];
            }

            float** getData()
            {
                if (channels_.empty())
                    return nullptr;

                return &channels_[0];
            }

        private:
            std::vector<float> data_;
            std::vector<float*> channels_;
        };

        private:
            int numChannels_ = 0;
            int numSamples_ = 0;

            MemoryBlock memoryBlock_;
            float* const* data_;
            std::vector<float*> channelData_;
        };
}

#pragma once

#include "core/IAudioBuffer.h"


namespace hidonash::core
{
    class AudioBuffer : public IAudioBuffer
    {
    public:
        AudioBuffer(int numChannels, int numSamples);

        AudioBuffer(float** dataToReferTo, int numChannels, int numSamples);

        ~AudioBuffer() override;

        float getSample(int channel, int sample) const override;

        void setSample(int destChannel, int destSample, float newValue) override;

        int getNumChannels() const override;

        int getNumSamples() const override;

        float* getDataPointer() const override;

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
            float** data_;
        };
}

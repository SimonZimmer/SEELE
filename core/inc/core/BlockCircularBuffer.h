#pragma once

#include <algorithm>

#include <core/AudioBuffer.h>

namespace hidonash
{
    template <typename T>
    class BlockCircularBuffer
    {
    public:
        explicit BlockCircularBuffer(size_t newSize)
        {
            setSize(newSize);
            reset();
        }

        void setReadHopSize(int hopSize)
        {
            readHopSize_ = hopSize;
        }

        void setWriteHopSize(int hopSize)
        {
            writeHopSize_ = hopSize;
        }

        void setSize(size_t newSize)
        {
            if (newSize == length_)
                return;

            block_.setSize(1, newSize);
            length_ = newSize;
            writeIndex_ = readIndex_ = 0;
        }

        void reset()
        {
            block_.fill(0.f);
            writeIndex_ = readIndex_ = 0;
        }

        T* getDataPointer()
        {
            return block_.getDataPointer();
        }

        void read(core::AudioBuffer<T>& destBuffer, int offset, const size_t destLength)
        {
            const auto firstReadAmount = readIndex_ + destLength >= length_ ?
                                         length_ - readIndex_ : destLength;

            destBuffer.copy(block_, readIndex_, offset, firstReadAmount);

            if(firstReadAmount < destLength)
                destBuffer.copy(block_, 0, firstReadAmount, destLength - firstReadAmount);

            readIndex_ += readHopSize_ != 0 ? readHopSize_ : destLength;
            readIndex_ %= length_;
        }

        void write(core::AudioBuffer<T>& sourceBuffer, const int offset, const size_t writeLength)
        {
            const auto firstWriteAmount = writeIndex_ + writeLength >= length_ ?
                                          length_ - writeIndex_ : writeLength;

            block_.copy(sourceBuffer, offset, writeIndex_, firstWriteAmount);

            if(firstWriteAmount < writeLength)
                block_.copy(sourceBuffer, firstWriteAmount + offset, writeIndex_, writeLength - firstWriteAmount);

            incrementWriteIndex(writeLength);
        }

        void overlapWrite(core::AudioBuffer<T>& sourceBuffer, const size_t writeLength)
        {
            const auto overlapAmount = getOverlapAmount(writeLength);
            auto tempWriteIndex = writeIndex_;
            auto firstWriteAmount = writeIndex_ + overlapAmount > length_ ? length_ - writeIndex_ : overlapAmount;

            block_.add(sourceBuffer, firstWriteAmount, 0, writeIndex_);

            if(firstWriteAmount < overlapAmount)
                block_.add(sourceBuffer, overlapAmount - firstWriteAmount, firstWriteAmount);

            tempWriteIndex += overlapAmount;
            tempWriteIndex %= length_;

            const auto remainingElements = writeLength - overlapAmount;
            firstWriteAmount = tempWriteIndex + remainingElements > length_ ?
                               length_ - tempWriteIndex : remainingElements;

            block_.copy(sourceBuffer, overlapAmount, tempWriteIndex, firstWriteAmount);

            if (firstWriteAmount < remainingElements)
                block_.copy(sourceBuffer, overlapAmount + firstWriteAmount, 0, (remainingElements - firstWriteAmount));

            incrementWriteIndex(writeLength);
        }

    private:
        size_t getOverlapAmount(size_t writeLength) const
        {
            const int writeIndexDifference = (writeIndex_ <= latestDataIndex_) ? latestDataIndex_ - writeIndex_ : length_ - writeIndex_ + latestDataIndex_;
            const int overlapSampleCount = writeLength - writeHopSize_;

            return std::min(writeIndexDifference, overlapSampleCount);
        }

        void incrementWriteIndex(size_t writeLength)
        {
            writeIndex_ += writeHopSize_ != 0 ? writeHopSize_ : writeLength;
            writeIndex_ %= length_;
            latestDataIndex_ = writeIndex_ + writeLength % length_;
        }

        core::AudioBuffer<T> block_{1, 0};
        int writeIndex_ = 0;
        int readIndex_ = 0;
        size_t length_ = 0;
        size_t latestDataIndex_ = 0;
        size_t writeHopSize_ = 0;
        size_t readHopSize_ = 0;
    };
}

#pragma once

#include <memory>

#include <core/AudioBuffer.h>

namespace sz
{
    namespace
    {
        int getDifferenceBetweenIndexes(int index1, int index2, int bufferLength)
        {
            return (index1 <= index2) ? index2 - index1 : bufferLength - index1 + index2;
        }
    }

    template<typename ElementType = float>
    struct BlockCircularBuffer final
    {
        BlockCircularBuffer(long newSize)
        {
            setSize(newSize, true);
        }

        void setReadHopSize(int hopSize)
        {
            readHopSize = hopSize;
        }

        void setWriteHopSize(int hopSize)
        {
            writeHopSize = hopSize;
        }

        void setSize(long newSize, bool shouldClear = false)
        {
            if (newSize == length)
            {
                if (shouldClear)
                    reset ();

                return;
            }

            block.setSize(1, newSize);
            length = newSize;
            writeIndex = readIndex = 0;
        }

        void reset()
        {
            block.fill(0.f);
            writeIndex = readIndex = 0;
        }

        // Read samples from the internal buffer into the 'destBuffer'
        // perform a wrap of the read if near the internal buffer boundaries
        void read(core::AudioBuffer<float>& destBuffer, int offset, const long destLength)
        {
            auto&& destBufferData = destBuffer.getDataPointer() + offset;
            const auto firstReadAmount = readIndex + destLength >= length ?
                                         length - readIndex : destLength;

            const auto internalBuffer = block.getDataPointer();

            memcpy(destBufferData, internalBuffer + readIndex, sizeof (ElementType) * firstReadAmount);

            if(firstReadAmount < destLength)
                memcpy(destBufferData + firstReadAmount, internalBuffer,
                       sizeof (ElementType) * (static_cast<unsigned long long>(destLength) - firstReadAmount));

            readIndex += readHopSize != 0 ? readHopSize : destLength;
            readIndex %= length;
        }

        // Write all samples from the 'sourceBuffer' into the internal buffer
        // Perform any wrapping required
        void write(core::AudioBuffer<float>& sourceBuffer, const int offset, const long writeLength)
        {
            auto&& sourceBufferData = sourceBuffer.getDataPointer() + offset;
            const auto firstWriteAmount = writeIndex + writeLength >= length ?
                                          length - writeIndex : writeLength;

            auto internalBuffer = block.getDataPointer();
            memcpy(internalBuffer + writeIndex, sourceBufferData, sizeof (ElementType) * firstWriteAmount);

            if(firstWriteAmount < writeLength)
                memcpy(internalBuffer, sourceBufferData + firstWriteAmount,
                       sizeof (ElementType) * (static_cast<unsigned long long>(writeLength) - firstWriteAmount));

            writeIndex += writeHopSize != 0 ? writeHopSize : writeLength;
            writeIndex %= length;

            latestDataIndex = writeIndex + writeLength % length;
        }

        // The first 'overlapAmount' of 'sourceBuffer' samples are added to the existing buffer
        // The remainder of samples are set in the buffer (overwrite)
        void overlapWrite(core::AudioBuffer<float>& sourceBuffer, const long writeLength)
        {
            auto&& sourceBufferData = sourceBuffer.getDataPointer();
            // Since we're using a circular buffer, we have to be careful when to add samples to the existing
            // data and when to overwrite out of date samples. This number can change when modulating between
            // the pitch (which alters the size of the overlaps). The calculation below will determine the
            // index we need to "add" to and at which point we need to "set" the samples to overwrite the history
            const int writeIndexDifference = getDifferenceBetweenIndexes(writeIndex, latestDataIndex, length);
            const int overlapSampleCount = writeLength - writeHopSize;
            const auto overlapAmount = std::min(writeIndexDifference, overlapSampleCount);

            auto tempWriteIndex = writeIndex;
            auto firstWriteAmount = writeIndex + overlapAmount > length ?
                                    length - writeIndex : overlapAmount;

            auto internalBuffer = block.getDataPointer();

            for(auto i = 0; i < firstWriteAmount; ++i)
                internalBuffer[i + writeIndex] += sourceBufferData[i];

            if(firstWriteAmount < overlapAmount)
                for(auto i = 0; i < (overlapAmount - firstWriteAmount); ++i)
                    internalBuffer[i] += sourceBufferData[i + firstWriteAmount];

            tempWriteIndex += overlapAmount;
            tempWriteIndex %= length;

            const auto remainingElements = writeLength - overlapAmount;
            firstWriteAmount = tempWriteIndex + remainingElements > length ?
                               length - tempWriteIndex : remainingElements;

            memcpy(internalBuffer + tempWriteIndex, sourceBufferData + overlapAmount,
                   sizeof (ElementType) * firstWriteAmount);

            if (firstWriteAmount < remainingElements)
                memcpy(internalBuffer, sourceBufferData + overlapAmount + firstWriteAmount,
                       sizeof (ElementType) * (remainingElements - static_cast<unsigned long long>(firstWriteAmount)));

            latestDataIndex = (writeIndex + writeLength) % length;
            writeIndex += writeHopSize;
            writeIndex %= length;
        }

    private:
        core::AudioBuffer<float> block{1, 0};
        long writeIndex = 0;
        long readIndex = 0;
        long length = 0;
        long latestDataIndex = 0;
        int writeHopSize = 0;
        int readHopSize = 0;
    };
}


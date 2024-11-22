// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Buffer.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Network
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Buffer::Buffer(UInt Capacity)
        : mMemory(Capacity, 0)
    {
        Reset();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Buffer::Reset()
    {
        mReader.store(0, std::memory_order_relaxed);
        mWriter.store(0, std::memory_order_relaxed);
        mMarker.store(0, std::memory_order_relaxed);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    CPtr<UInt8> Buffer::Reserve(UInt Length)
    {
        const UInt Writer = mWriter.load(std::memory_order_acquire);
        const UInt Reader = mReader.load(std::memory_order_acquire);
        const UInt Marker = mMarker.load(std::memory_order_acquire);

        if (Marker != 0)
        {
            if (Writer + Length <= Reader)
            {
                return CPtr<UInt8>(mMemory.data() + Writer, Length);
            }
        }
        else
        {
            if (Writer + Length <= mMemory.size())
            {
                return CPtr<UInt8>(mMemory.data() + Writer, Length);
            }
            else
            {
                if (Reader >= Length)
                {
                    return CPtr<UInt8>(mMemory.data(), Length);
                }
            }
        }
        return CPtr<UInt8>();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Buffer::Commit(UInt Length)
    {
        const UInt Writer = mWriter.load(std::memory_order_acquire);

        if (mWriter + Length > mMemory.size())
        {
            mMarker.store(Writer, std::memory_order_release);
            mWriter.store(Length, std::memory_order_release);
        }
        else
        {
            mWriter.store(Writer + Length, std::memory_order_release);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    CPtr<UInt8> Buffer::Read()
    {
        const UInt Reader = mReader.load(std::memory_order_acquire);
        const UInt Marker = mMarker.load(std::memory_order_acquire);

        if (Marker != 0)
        {
            if (Reader == Marker)
            {
                const UInt Writer = mWriter.load(std::memory_order_acquire);
                return CPtr<UInt8>(mMemory.data(), Writer);
            }
            else
            {
                return CPtr<UInt8>(mMemory.data() + Reader, Marker - Reader);
            }
        }
        else
        {
            const UInt Writer = mWriter.load(std::memory_order_acquire);
            return CPtr<UInt8>(mMemory.data() + Reader, Writer - Reader);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Buffer::Consume(CPtr<UInt8> Block)
    {
        const UInt Reader = mReader.load(std::memory_order_acquire);
        const UInt Marker = mMarker.load(std::memory_order_acquire);
        const UInt Offset = static_cast<UInt>(Block.data() - mMemory.data() + Block.size());

        if (Offset == Marker)
        {
            mReader.store(0, std::memory_order_release);
            mMarker.store(0, std::memory_order_release);
        }
        else
        {
            if (Reader >= Offset)
            {
                mMarker.store(0, std::memory_order_release);
            }
            mReader.store(Offset, std::memory_order_release);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Buffer::IsFull() const
    {
        const UInt Marker = mMarker.load(std::memory_order_acquire);
        if (Marker != 0)
        {
            const UInt Writer = mWriter.load(std::memory_order_acquire);
            const UInt Reader = mReader.load(std::memory_order_acquire);
            return (Reader == Writer);
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Buffer::IsEmpty() const
    {
        const UInt32 Writer = mWriter.load(std::memory_order_acquire);
        if (Writer == 0)
        {
            const UInt32 Marker = mMarker.load(std::memory_order_acquire);
            return (Marker == 0);
        }
        return false;
    }
}
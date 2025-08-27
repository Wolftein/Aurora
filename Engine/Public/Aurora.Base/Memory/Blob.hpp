// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Aurora.Base/Utility.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief A move-only binary data payload with customizable destruction.
    /// 
    /// `Blob` is a lightweight container for raw binary memory, intended for one-time
    /// ownership transfers. It supports dynamically allocated buffers, externally managed
    /// memory, or custom cleanup logic via user-provided deleters.
    /// 
    /// Typical use cases include file contents, network packets, or serialized objects.
    class Blob final
    {
    public:

        /// \brief Function pointer type used to destroy the payload.
        using Deleter = void(*)(Ptr<void>);

        /// \brief A no-op deleter for externally managed memory.
        constexpr static auto kEmptyDeleter = [](Ptr<void>)
        {
        };

        /// \brief A default deleter for heap-allocated arrays.
        /// 
        /// \tparam Type The element type originally allocated.
        template<typename Type>
        constexpr static auto kBasicDeleter = [](Ptr<void> Address)
        {
            delete[] static_cast<Ptr<Type>>(Address);
        };

    public:

        /// \brief Constructs an empty payload.
        AURORA_INLINE Blob()
            : mPointer { nullptr },
              mSize    { 0 },
              mDeleter { nullptr }
        {
        }

        /// \brief Constructs a heap-allocated payload of the specified size (in bytes).
        /// 
        /// The payload owns a raw buffer initialized via `new[]` and automatically
        /// frees it upon destruction.
        /// 
        /// \param Size The number of bytes to allocate.
        AURORA_INLINE Blob(UInt32 Size)
            : mPointer { new Byte[Size] },
              mSize    { Size },
              mDeleter { kBasicDeleter<Byte> }
        {
        }

        /// \brief Constructs a payload using externally managed memory and a custom deleter.
        /// 
        /// \param Pointer The raw pointer to the memory block.
        /// \param Size    Size of the memory block in bytes.
        /// \param Deleter A function to release the memory when done.
        template<typename Type>
        AURORA_INLINE Blob(Type Pointer, UInt32 Size, Deleter Deleter)
            : mPointer { reinterpret_cast<Ptr<Byte>>(Pointer) },
              mSize    { Size },
              mDeleter { Deleter }
        {
        }

        /// \brief Move constructor.
        AURORA_INLINE Blob(AnyRef<Blob> Other)
            : mPointer { Exchange(Other.mPointer, nullptr) },
              mSize    { Exchange(Other.mSize, 0) },
              mDeleter { Exchange(Other.mDeleter, nullptr) }
        {
        }

        /// \brief Deleted copy constructor.
        AURORA_INLINE Blob(ConstRef<Blob>) = delete;

        /// \brief Destructor. Releases the memory if owned.
        AURORA_INLINE ~Blob()
        {
            Reset();
        }

        /// \brief Releases the memory if owned.
        AURORA_INLINE void Reset()
        {
            if (mPointer)
            {
                mDeleter(mPointer);
            }
            mPointer = nullptr;
        }

        /// \brief Returns a mutable pointer to the payload data.
        /// 
        /// \param Offset Optional byte offset (in elements).
        /// \return A pointer to the data, cast to the specified type.
        template<typename Type = void>
        AURORA_INLINE Ptr<Type> GetData(UInt32 Offset = 0)
        {
            return reinterpret_cast<Ptr<Type>>(mPointer + Offset);
        }

        /// \brief Returns a read-only pointer to the payload data.
        /// 
        /// \param Offset Optional byte offset (in elements).
        /// \return A const pointer to the data.
        template<typename Type = void>
        AURORA_INLINE ConstPtr<Type> GetData(UInt32 Offset = 0) const
        {
            return reinterpret_cast<ConstPtr<Type>>(mPointer + Offset);
        }

        /// \brief Returns a mutable span over the payload data.
        /// 
        /// \param Offset Optional byte offset (in elements).
        /// \return A span representing a view over the memory.
        template<typename Type>
        AURORA_INLINE Span<Type> GetSpan(UInt32 Offset = 0)
        {
            return Span<Type>(static_cast<Ptr<Type>>(mPointer) + Offset, mSize / sizeof(Type));
        }

        /// \brief Returns a read-only span over the payload data.
        /// 
        /// \param Offset Optional byte offset (in elements).
        /// \return A const span representing a view over the memory.
        template<typename Type>
        AURORA_INLINE ConstSpan<Type> GetSpan(UInt32 Offset = 0) const
        {
            return ConstSpan<Type>(static_cast<ConstPtr<Type>>(mPointer) + Offset, mSize / sizeof(Type));
        }

        /// \brief Returns a read-only text representation of the payload.
        /// 
        /// \return A textual view over the payload buffer.
        AURORA_INLINE ConstText GetText() const
        {
            return ConstText(reinterpret_cast<Ptr<Char>>(mPointer), mSize);
        }

        /// \brief Returns the size of the payload in bytes.
        /// 
        /// \return The total number of bytes.
        AURORA_INLINE UInt32 GetSize() const
        {
            return mSize;
        }

        /// \brief Copies raw data into the payload buffer at the specified offset.
        /// 
        /// \param Source Pointer to the source data.
        /// \param Size   The number of bytes to copy.
        /// \param Offset The byte offset within the buffer to begin writing to.
        template<typename Type>
        AURORA_INLINE void Copy(ConstPtr<Type> Source, UInt32 Size, UInt32 Offset = 0)
        {
            std::memcpy(mPointer + Offset, Source, Size);
        }

        /// \brief Transfers ownership of resources from another object.
        ///
        /// \param Other The source object to move resources from.
        /// \return A reference to this object after the ownership transfer.
        AURORA_INLINE Ref<Blob> operator=(AnyRef<Blob> Other) noexcept
        {
            if (this != &Other)
            {
                if (mPointer)
                {
                    mDeleter(mPointer);
                }

                mPointer = Exchange(Other.mPointer, nullptr);
                mSize    = Exchange(Other.mSize, 0);
                mDeleter = Exchange(Other.mDeleter, nullptr);
            }
            return (*this);
        }

        /// \brief Deleted copy assignment operator.
        AURORA_INLINE Ref<Blob> operator=(ConstRef<Blob>) = delete;

        /// \brief Compares this payload with another for equality.
        /// 
        /// \param Other The other payload to compare.
        /// \return `true` if both refer to the same data and size, `false` otherwise.
        AURORA_INLINE constexpr Bool operator==(ConstRef<Blob> Other) const
        {
            return mPointer == Other.mPointer && mSize == Other.mSize;
        }

        /// \brief Compares this payload with another for inequality.
        /// 
        /// \param Other The other payload to compare.
        /// \return `true` if data or size differ, `false` otherwise.
        AURORA_INLINE constexpr Bool operator!=(ConstRef<Blob> Other) const
        {
            return !(*this == Other);
        }

        /// \brief Checks if the payload is null.
        /// 
        /// \return `true` if no data is held, `false` otherwise.
        AURORA_INLINE constexpr Bool operator==(std::nullptr_t) const
        {
            return mPointer == nullptr;
        }

        /// \brief Checks if the payload is valid (non-null).
        /// 
        /// \return `true` if the payload contains data, `false` otherwise.
        AURORA_INLINE constexpr Bool operator!=(std::nullptr_t) const
        {
            return mPointer != nullptr;
        }

        /// \brief Checks whether this payload holds valid memory.
        /// 
        /// \return `true` if the payload contains a valid pointer, `false` otherwise.
        AURORA_INLINE constexpr operator Bool() const
        {
            return mPointer != nullptr;
        }

        /// \brief Implicitly converts the object to a constant byte span.
        ///
        /// \return Constant span over the object’s byte data.
        AURORA_INLINE operator ConstSpan<Byte>() const
        {
            return GetSpan<Byte>();
        }

    public:

        template<typename Type>
        AURORA_INLINE static constexpr Blob FromImmutable(std::initializer_list<Type> Data)
        {

            return Blob(const_cast<Ptr<Type>>(Data.begin()), Data.size() * sizeof(Type), kEmptyDeleter);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<Byte> mPointer;
        UInt32    mSize;
        Deleter   mDeleter;
    };
}
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

#include "Aurora.Base/Collection.hpp"
#include "Aurora.Base/Enum.hpp"
#include "Aurora.Base/Utility.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Base
{
    /// \brief Binary data writer for in-memory data.
    class Writer final
    {
    public:

        /// \brief Creates an empty writer.
        AURORA_INLINE Writer()
            : mOffset { 0 }
        {
        }

        /// \brief Constructs a writer with a specified initial capacity.
        ///
        /// \param Capacity The number of bytes to allocate for the buffer.
        AURORA_INLINE explicit Writer(UInt32 Capacity)
            : mBuffer (Capacity, 0),
              mOffset { 0 }
        {
        }

        /// \brief Returns a span of the current written data.
        ///
        /// \return Span representing the valid portion of the buffer.
        AURORA_INLINE Span<Byte> GetData()
        {
            return Span<Byte>(mBuffer.data(), mOffset);
        }

        /// \brief Returns the total capacity of the buffer.
        ///
        /// \return Capacity in bytes.
        AURORA_INLINE UInt32 GetCapacity() const
        {
            return mBuffer.size();
        }

        /// \brief Returns the number of bytes still available for writing.
        ///
        /// \return Number of unused bytes remaining in the buffer.
        AURORA_INLINE UInt32 GetAvailable() const
        {
            return GetCapacity() - mOffset;
        }

        /// \brief Returns the current write position.
        ///
        /// \return Current offset in the buffer.
        AURORA_INLINE UInt32 GetOffset() const
        {
            return mOffset;
        }

        /// \brief Ensures the buffer has enough space to accommodate a write.
        ///
        /// \param Length Number of bytes required.
        void Ensure(UInt32 Length)
        {
            static constexpr UInt32 kPageAllocation = 512;

            if (const UInt32 Capacity = GetCapacity(); mOffset + Length >= Capacity)
            {
                const UInt32 Amount = (mOffset + Length - Capacity);
                mBuffer.resize(Capacity + (Amount / kPageAllocation + 1) * kPageAllocation);
            }
        }

        /// \brief Clears the writer's offset, effectively resetting the write position.
        AURORA_INLINE void Clear()
        {
            mOffset = 0u;
        }

        /// \brief Reserve a byte range from the buffer with alignment.
        ///
        /// \param Length Size of the region in bytes.
        /// \param Stride Alignment in bytes to satisfy before the reservation.
        /// \return Pointer to the start of the reserved region.
        template<typename Type>
        AURORA_INLINE Ptr<Type> Reserve(UInt32 Length, UInt32 Stride)
        {
            const UInt Padding = Align(mOffset, Stride) - mOffset;
            Ensure(Padding + Length);

            mOffset += Padding + Length;

            return reinterpret_cast<Ptr<Type>>(&mBuffer[mOffset - Length]);
        }

        /// \brief Writes raw data into the buffer with optional alignment.
        ///
        /// \param Value  When `Type` is a pointer: address of the source bytes.
        /// \param Length Number of bytes to write.
        template<typename Type, UInt32 Alignment = 1>
        AURORA_INLINE void Write(Type Value, UInt32 Length = sizeof(Type))
        {
            if constexpr (Alignment > 1)
            {
                const UInt Padding = Align<Alignment>(mOffset) - mOffset;
                Ensure(Padding + Length);

                mOffset += Padding;
            }
            else
            {
                Ensure(Length);
            }

            if constexpr (IsPointer<Type>)
            {
                std::memcpy(mBuffer.data() + mOffset, Value, Length);
            }
            else
            {
                reinterpret_cast<Ref<Type>>(mBuffer[mOffset]) = Value;
            }

            mOffset += Length;
        }

        /// \brief Writes a boolean value.
        ///
        /// \param Value The boolean value to write.
        AURORA_INLINE void WriteBool(Bool Value)
        {
            Write<Bool>(Value);
        }

        /// \brief Writes an enumeration as its underlying integral type.
        ///
        /// \param Value The enumeration to write.
        template<typename Type>
        AURORA_INLINE void WriteEnum(Type Value)
        {
            WriteInt(Enum::Cast(Value));
        }

        /// \brief Writes an integer using variable-length encoding.
        ///
        /// \param Value The integer value to write.
        template<typename Type>
        AURORA_INLINE void WriteInt(Type Value)
        {
            while (Value > 0x7F)
            {
                WriteUInt8(static_cast<UInt8>(Value & 0x7Fu) | 0x80u);
                Value >>= 7u;
            }
            WriteUInt8(static_cast<UInt8>(Value) & 0x7Fu);
        }

        /// \brief Writes a signed 8-bit integer.
        ///
        /// \param Value The 8-bit signed integer to write.
        AURORA_INLINE void WriteInt8(SInt8 Value)
        {
            Write<SInt8>(Value);
        }

        /// \brief Writes an unsigned 8-bit integer.
        ///
        /// \param Value The 8-bit unsigned integer to write.
        AURORA_INLINE void WriteUInt8(UInt8 Value)
        {
            Write<UInt8>(Value);
        }

        /// \brief Writes a signed 16-bit integer.
        ///
        /// \param Value The 16-bit signed integer to write.
        AURORA_INLINE void WriteInt16(SInt16 Value)
        {
            Write<SInt16>(Value);
        }

        /// \brief Writes an unsigned 16-bit integer.
        ///
        /// \param Value The 16-bit unsigned integer to write.
        AURORA_INLINE void WriteUInt16(UInt16 Value)
        {
            Write<UInt16>(Value);
        }

        /// \brief Writes a signed 32-bit integer.
        ///
        /// \param Value The 32-bit signed integer to write.
        AURORA_INLINE void WriteInt32(SInt32 Value)
        {
            Write<SInt32>(Value);
        }

        /// \brief Writes an unsigned 32-bit integer.
        ///
        /// \param Value The 32-bit unsigned integer to write.
        AURORA_INLINE void WriteUInt32(UInt32 Value)
        {
            Write<UInt32>(Value);
        }

        /// \brief Writes a signed 64-bit integer.
        ///
        /// \param Value The 64-bit signed integer to write.
        AURORA_INLINE void WriteInt64(SInt64 Value)
        {
            Write<SInt64>(Value);
        }

        /// \brief Writes an unsigned 64-bit integer.
        ///
        /// \param Value The 64-bit unsigned integer to write.
        AURORA_INLINE void WriteUInt64(UInt64 Value)
        {
            Write<UInt64>(Value);
        }

        /// \brief Writes a 32-bit floating point number.
        ///
        /// \param Value The 32-bit floating point number to write.
        AURORA_INLINE void WriteReal32(Real32 Value)
        {
            Write<Real32>(Value);
        }

        /// \brief Writes a 64-bit floating point number.
        ///
        /// \param Value The 64-bit floating point number to write.
        AURORA_INLINE void WriteReal64(Real64 Value)
        {
            Write<Real64>(Value);
        }

        /// \brief Writes a string prefixed with its character count.
        ///
        /// \param Value The string to write.
        AURORA_INLINE void WriteText(ConstText Value)
        {
            WriteInt(Value.size());

            if (!Value.empty())
            {
                Write<ConstPtr<Char>>(Value.data(), Value.size() * sizeof(Char));
            }
        }

        /// \brief Writes a plain old data (POD) type into the buffer with proper alignment.
        ///
        /// \tparam Type      The POD type to write (must be trivially constructible).
        /// \tparam Arguments Constructor argument types used to initialize \p Type.
        /// \param Parameters Arguments forwarded to construct the instance of \p Type in place.
        template<typename Type, typename... Arguments>
        AURORA_INLINE void WriteStruct(AnyRef<Arguments>... Parameters)
        {
            const UInt Padding = Align<alignof(Type)>(mOffset) - mOffset;
            Ensure(Padding + sizeof(Type));

            mOffset += Padding;

            InPlaceCreate<Type>(mBuffer.data() + mOffset, Forward<Arguments>(Parameters)...);

            mOffset += sizeof(Type);
        }

        /// \brief Writes a contiguous block of trivially copyable elements into the buffer.
        ///
        /// \param Value The view of elements to write.
        template<typename Type>
        AURORA_INLINE void WriteBlock(Span<Type> Value)
        {
            WriteInt<UInt32>(Value.size());

            if (!Value.empty())
            {
                Write<ConstPtr<Type>>(Value.data(), Value.size_bytes());
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<Byte> mBuffer;
        UInt32       mOffset;
    };
}
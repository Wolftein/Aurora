// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Aurora.Base/Trait.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    class Writer final
    {
    public:

        // -=(Undocumented)=-
        static constexpr inline UInt32 kDefaultCapacity  = 1024;

        // -=(Undocumented)=-
        static constexpr inline UInt32 kDefaultAlignment = 1;

    public:

        // -=(Undocumented)=-
        explicit Writer(UInt32 Capacity = kDefaultCapacity, UInt32 Alignment = kDefaultAlignment)
            : mBuffer    (Capacity, 0),
              mOffset    { 0 },
              mAlignment { Alignment }
        {
        }

        // -=(Undocumented)=-
        CPtr<UInt8> GetData()
        {
            return CPtr<UInt8>(mBuffer.data(), mOffset);
        }

        // -=(Undocumented)=-
        UInt32 GetCapacity() const
        {
            return mBuffer.size();
        }

        // -=(Undocumented)=-
        UInt32 GetAvailable() const
        {
            return GetCapacity() - mOffset;
        }

        // -=(Undocumented)=-
        UInt32 GetOffset() const
        {
            return mOffset;
        }

        // -=(Undocumented)=-
        UInt32 GetAlignment() const
        {
            return mAlignment;
        }

        // -=(Undocumented)=-
        Bool HasData() const
        {
            return (mOffset > 0);
        }

        // -=(Undocumented)=-
        void Ensure(UInt32 Length)
        {
            const UInt32 Capacity = GetCapacity();

            if (mOffset + Length >= Capacity)
            {
                const UInt32 Amount = (mOffset + Length - Capacity);
                mBuffer.resize(Capacity + (Amount / kDefaultCapacity + 1) * kDefaultCapacity);
            }
        }

        // -=(Undocumented)=-
        void Clear()
        {
            mOffset = 0u;
        }

        // -=(Undocumented)=
        template<typename Type>
        Ptr<Type> Reserve(UInt32 Length = sizeof(Type))
        {
            Ensure(Length);

            Ptr<Type> Pointer = reinterpret_cast<Ptr<Type>>(AddressOf(mBuffer[mOffset]));
            mOffset = Align(mOffset + Length, mAlignment);
            return Pointer;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Write(Type Value, UInt32 Length = sizeof(Type))
        {
            Ensure(Length);

            if constexpr (std::is_pointer_v<Type>)
            {
                std::memcpy(& mBuffer[mOffset], Value, Length);
            }
            else
            {
                reinterpret_cast<Ref<Type>>(mBuffer[mOffset]) = Value;
            }

            mOffset = Align(mOffset + Length, mAlignment);
        }

        // -=(Undocumented)=-
        template<typename Type>
        void WriteBlock(CPtr<const Type> Value)
        {
            WriteInt<UInt32>(Value.size());

            if (!Value.empty())
            {
                Write<Ptr<const Type>>(Value.data(), Value.size_bytes());
            }
        }

        // -=(Undocumented)=-
        void WriteBool(Bool Value)
        {
            Write<Bool>(Value);
        }

        // -=(Undocumented)=-
        template<typename Type>
        void WriteEnum(Type Value)
        {
            WriteInt(static_cast<std::underlying_type_t<Type>>(Value));
        }

        // -=(Undocumented)=-
        template<typename Type, typename = std::enable_if_t<std::is_integral_v<Type>>>
        void WriteInt(Type Value)
        {
            while (Value > 0x7F)
            {
                WriteUInt8(static_cast<UInt8>(Value & 0x7Fu) | 0x80u);
                Value >>= 7u;
            }
            WriteUInt8(static_cast<UInt8>(Value) & 0x7Fu);
        }

        // -=(Undocumented)=-
        void WriteInt8(SInt8 Value)
        {
            Write<SInt8>(Value);
        }

        // -=(Undocumented)=-
        void WriteUInt8(UInt8 Value)
        {
            Write<UInt8>(Value);
        }

        // -=(Undocumented)=-
        void WriteInt16(SInt16 Value)
        {
            Write<SInt16>(Value);
        }

        // -=(Undocumented)=-
        void WriteUInt16(UInt16 Value)
        {
            Write<UInt16>(Value);
        }

        // -=(Undocumented)=-
        void WriteInt32(SInt32 Value)
        {
            Write<SInt32>(Value);
        }

        // -=(Undocumented)=-
        void WriteUInt32(UInt32 Value)
        {
            Write<UInt32>(Value);
        }

        // -=(Undocumented)=-
        void WriteInt64(SInt64 Value)
        {
            Write<SInt64>(Value);
        }

        // -=(Undocumented)=-
        void WriteUInt64(UInt64 Value)
        {
            Write<UInt64>(Value);
        }

        // -=(Undocumented)=-
        void WriteReal32(Real32 Value)
        {
            Write<Real32>(Value);
        }

        // -=(Undocumented)=-
        void WriteReal64(Real64 Value)
        {
            Write<Real64>(Value);
        }

        // -=(Undocumented)=-
        void WriteString8(CStr Value)
        {
            WriteInt(Value.size());

            if (!Value.empty())
            {
                Write<CStr::const_pointer>(Value.data(), Value.size() * sizeof(CStr::value_type));
            }
        }

        // -=(Undocumented)=-
        void WriteString16(CStr16 Value)
        {
            WriteInt(Value.size());

            if (!Value.empty())
            {
                Write<CStr16::const_pointer>(Value.data(), Value.size() * sizeof(CStr16::value_type));
            }
        }

        // -=(Undocumented)=-
        template<typename Type, typename = std::enable_if_t<std::is_arithmetic<Type>::value>>
        void WriteNumber(Type Value)
        {
            if constexpr (std::is_integral_v<Type>)
            {
                WriteInt<Type>(Value);
            }
            else
            {
                Write<Type>(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Type, typename Decay = std::decay_t<Type>>
        void WriteObject(ConstRef<Type> Value)
        {
            if constexpr (std::is_same_v<Decay, Bool>)
            {
                WriteBool(Value);
            }
            else if constexpr (std::is_arithmetic_v<Decay>)
            {
                WriteNumber<Decay>(Value);
            }
            else if constexpr (std::is_same_v<Decay, SStr> || std::is_same_v<Decay, CStr>)
            {
                WriteString8(Value);
            }
            else if constexpr (std::is_same_v<Decay, SStr16> || std::is_same_v<Decay, CStr16>)
            {
                WriteString16(Value);
            }
            else
            {
                // Cast away const-ness to enable calling the 'OnSerialize' function, which supports both
                // read and write operations.
                const_cast<Ref<Decay>>(Value).template OnSerialize<Writer>(*this);
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<UInt8> mBuffer;
        UInt32        mOffset;
        UInt32        mAlignment;
    };
}
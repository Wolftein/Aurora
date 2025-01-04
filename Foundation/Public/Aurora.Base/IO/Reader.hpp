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

#include "Aurora.Base/Trait.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    class Reader final
    {
    public:

        // -=(Undocumented)=-
        static constexpr inline UInt32 kDefaultAlignment = 1;

    public:

        // -=(Undocumented)=-
        explicit Reader()
            : mBuffer { nullptr },
              mLength { 0 },
              mOffset { 0 }
        {
        }

        // -=(Undocumented)=-
        template<typename Type>
        explicit Reader(Ptr<Type> Pointer, UInt32 Length, UInt32 Alignment = kDefaultAlignment)
            : mBuffer    { reinterpret_cast<Ptr<UInt8>>(Pointer) },
              mLength    { Length * sizeof(Type) },
              mOffset    { 0 },
              mAlignment { Alignment }
        {
        }

        // -=(Undocumented)=-
        template<typename Type>
        explicit Reader(CPtr<Type> Block, UInt32 Alignment = kDefaultAlignment)
            : mBuffer    { reinterpret_cast<Ptr<UInt8>>(Block.data()) },
              mLength    { static_cast<UInt32>(Block.size_bytes()) },
              mOffset    { 0 },
              mAlignment { Alignment }
        {
        }

        // -=(Undocumented)=-
        CPtr<UInt8> GetData()
        {
            return CPtr<UInt8>(mBuffer, mLength);
        }

        // -=(Undocumented)=-
        UInt32 GetCapacity() const
        {
            return mLength;
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
        void Skip(UInt32 Length)
        {
            mOffset += (mOffset + Length <= mLength ? Length : 0);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Type Peek(UInt32 Length = sizeof(Type))
        {
            if (mOffset + Length > mLength)
            {
                return {};
            }

            if constexpr (std::is_pointer_v<Type>)
            {
                return reinterpret_cast<Type>(mBuffer + mOffset);
            }
            else
            {
                return * reinterpret_cast<Ptr<Type>>(mBuffer + mOffset);
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        Type Read(UInt32 Length = sizeof(Type))
        {
            const Type Result = Peek<Type>(Length);
            mOffset = Align(mOffset + Length, mAlignment);
            return Result;
        }

        // -=(Undocumented)=-
        template<typename Type>
        CPtr<Type> ReadBlock()
        {
            const auto Size = ReadInt<UInt32>();
            const auto Data = Read<Ptr<Type>>(Size * sizeof(Type));
            return CPtr<Type>(Data, Size);
        }

        // -=(Undocumented)=-
        Bool ReadBool()
        {
            return Read<Bool>();
        }

        // -=(Undocumented)=-
        template<typename Type>
        Type ReadEnum()
        {
            return static_cast<Type>(ReadInt<std::underlying_type_t<Type>>());
        }

        // -=(Undocumented)=-
        template<typename Type, typename = std::enable_if_t<std::is_integral<Type>::value>>
        Type ReadInt()
        {
            Type   Result   = 0u;
            UInt32 Position = 0u;

            do
            {
                Result |= static_cast<Type>((Peek<UInt8>() & 0x7Fu)) << (7u * Position++);
            } while (ReadUInt8() & 0x80u);

            return Result;
        }

        // -=(Undocumented)=-
        SInt8 ReadInt8()
        {
            return Read<SInt8>();
        }

        // -=(Undocumented)=-
        UInt8 ReadUInt8()
        {
            return Read<UInt8>();
        }

        // -=(Undocumented)=-
        SInt16 ReadInt16()
        {
            return Read<SInt16>();
        }

        // -=(Undocumented)=-
        UInt16 ReadUInt16()
        {
            return Read<UInt16>();
        }

        // -=(Undocumented)=-
        SInt32 ReadInt32()
        {
            return Read<SInt32>();
        }

        // -=(Undocumented)=-
        UInt32 ReadUInt32()
        {
            return Read<UInt32>();
        }

        // -=(Undocumented)=-
        SInt64 ReadInt64()
        {
            return Read<SInt64>();
        }

        // -=(Undocumented)=-
        UInt64 ReadUInt64()
        {
            return Read<UInt64>();
        }

        // -=(Undocumented)=-
        Real32 ReadReal32()
        {
            return Read<Real32>();
        }

        // -=(Undocumented)=-
        Real64 ReadReal64()
        {
            return Read<Real64>();
        }

        // -=(Undocumented)=-
        CStr ReadString8()
        {
            const auto Size = ReadInt<UInt32>();
            const auto Data = Read<CStr::const_pointer>(Size * sizeof(CStr::value_type));
            return CStr(Data, Size);
        }

        // -=(Undocumented)=-
        CStr16 ReadString16()
        {
            const auto Size = ReadInt<UInt32>();
            const auto Data = Read<CStr16::const_pointer>(Size * sizeof(CStr16::value_type));
            return CStr16(Data, Size);
        }

        // -=(Undocumented)=-
        template<typename Type, typename = std::enable_if_t<std::is_arithmetic<Type>::value>>
        Type ReadNumber()
        {
            if constexpr (std::is_integral_v<Type>)
            {
                return ReadInt<Type>();
            }
            else
            {
                return Read<Type>();
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        Type ReadObject()
        {
            if constexpr (std::is_arithmetic_v<Type>)
            {
                return ReadNumber<Type>();
            }
            else if constexpr (std::is_same_v<Type, SStr> || std::is_same_v<Type, CStr>)
            {
                return ReadString8();
            }
            else if constexpr (std::is_same_v<Type, SStr16> || std::is_same_v<Type, CStr16>)
            {
                return ReadString16();
            }
            else
            {
                Type Result;
                Result.template OnSerialize<Reader>(* this);
                return Result;
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<UInt8> mBuffer;
        UInt32     mLength;
        UInt32     mOffset;
        UInt32     mAlignment;
    };
}
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

#include "Aurora.Base/Type.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace IO
{
    // -=(Undocumented)=-
    class Reader final
    {
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
        explicit Reader(Ptr<Type> Pointer, UInt Length)
            : mBuffer { reinterpret_cast<Ptr<UInt8>>(Pointer) },
              mLength { Length * sizeof(Type) },
              mOffset { 0 }
        {
        }

        // -=(Undocumented)=-
        template<typename Type>
        explicit Reader(CPtr<Type> Block)
            : mBuffer { reinterpret_cast<Ptr<UInt8>>(Block.data()) },
              mLength { Block.size_bytes() },
              mOffset { 0 }
        {
        }

        // -=(Undocumented)=-
        CPtr<UInt8> GetData()
        {
            return CPtr<UInt8>(mBuffer, mLength);
        }

        // -=(Undocumented)=-
        UInt GetCapacity() const
        {
            return mLength;
        }

        // -=(Undocumented)=-
        UInt GetAvailable() const
        {
            return GetCapacity() - mOffset;
        }

        // -=(Undocumented)=-
        UInt GetOffset() const
        {
            return mOffset;
        }

        // -=(Undocumented)=-
        void Skip(UInt Length)
        {
            mOffset += (mOffset + Length <= mLength ? Length : 0);
        }

        // -=(Undocumented)=-
        template<typename Type>
        Type Peek(UInt Length = sizeof(Type))
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
        Type Read(UInt Length = sizeof(Type))
        {
            const Type Result = Peek<Type>(Length);
            mOffset += Length;
            return Result;
        }

        // -=(Undocumented)=-
        template<typename Type>
        Type ReadAny()
        {
            if constexpr (std::is_arithmetic_v<Type>)
            {
                ReadNumber<Type>();
            }
            else if constexpr (std::is_same<Type, SStr>::value)
            {
                ReadString();
            }
            else
            {
                ReadObject<Type>();
            }
        }

        // -=(Undocumented)=-
        Bool ReadBool()
        {
            return Read<Bool>();
        }

        // -=(Undocumented)=-
        template<typename Type, typename = std::enable_if_t<std::is_integral<Type>::value>>
        Type ReadInt()
        {
            Type Result   = 0u;
            UInt Position = 0u;

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
        CStr ReadString()
        {
            const auto Size = ReadInt<UInt>();
            const auto Data = Read<CStr::const_pointer>(Size * sizeof(CStr::value_type));

            return CStr(Data, Size);
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
            return Type::Read(* this);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<UInt8> mBuffer;
        UInt       mLength;
        UInt       mOffset;
    };
}
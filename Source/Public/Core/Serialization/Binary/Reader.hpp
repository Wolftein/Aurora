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

#include "Core/Types.hpp"

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
        template<typename Type>
        explicit Reader(Ptr<Type> Pointer, UInt Length)
            : mBuffer { reinterpret_cast<Ptr<UInt08>>(Pointer) },
              mLength { Length * sizeof(Type) },
              mOffset { 0 }
        {
        }

        // -=(Undocumented)=-
        template<typename Type>
        explicit Reader(CPtr<Type> Block)
            : mBuffer { reinterpret_cast<Ptr<UInt08>>(Block.data()) },
              mLength { Block.size_bytes() },
              mOffset { 0 }
        {
        }

        // -=(Undocumented)=-
        CPtr<UInt08> GetData()
        {
            return { mBuffer, mLength };
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
        Bool ReadBool()
        {
            return Read<Bool>();
        }

        // -=(Undocumented)=-
        template<typename Type>
        Type ReadInt()
        {
            Type Result   = 0u;
            UInt Position = 0u;

            do
            {
                Result |= static_cast<Type>((Peek<UInt08>() & 0x7Fu)) << (7u * Position++);
            } while (ReadUInt8() & 0x80u);

            return Result;
        }

        // -=(Undocumented)=-
        SInt08 ReadInt8()
        {
            return Read<SInt08>();
        }

        // -=(Undocumented)=-
        UInt08 ReadUInt8()
        {
            return Read<UInt08>();
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
            const auto Size = ReadInt<UInt>();
            const auto Data = Read<CStr::const_pointer>(Size * sizeof(CStr::value_type));

            return CStr(Data, Size);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ptr<UInt08> mBuffer;
        UInt        mLength;
        UInt        mOffset;
    };
}
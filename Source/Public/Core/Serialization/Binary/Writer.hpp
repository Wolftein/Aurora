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

#include "Core/Trait.hpp"

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
        static constexpr inline UInt32 kDefaultCapacity = 1024;

    public:

        // -=(Undocumented)=-
        explicit Writer(UInt Capacity = kDefaultCapacity)
            : mBuffer   ( Capacity, 0 ),
              mOffset   { 0 }
        {
        }

        // -=(Undocumented)=-
        CPtr<UInt08> GetData()
        {
            return CPtr<UInt08>(mBuffer.data(), mOffset);
        }

        // -=(Undocumented)=-
        UInt GetCapacity() const
        {
            return mBuffer.size();
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
        Bool HasData() const
        {
            return (mOffset > 0);
        }

        // -=(Undocumented)=-
        void Ensure(UInt Length)
        {
            const UInt Capacity = GetCapacity();

            if (mOffset + Length >= Capacity)
            {
                const UInt Amount = (mOffset + Length - Capacity);
                mBuffer.resize(Capacity + (Amount / kDefaultCapacity + 1) * kDefaultCapacity);
            }
        }

        // -=(Undocumented)=-
        void Clear()
        {
            mOffset = 0u;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void Write(Type Data, UInt Length = sizeof(Type))
        {
            Ensure(Length);

            if constexpr (std::is_pointer_v<Type>)
            {
                FastCopyMemory(& mBuffer[mOffset], Data, Length);
            }
            else
            {
                reinterpret_cast<Ref<Type>>(mBuffer[mOffset]) = Data;
            }
            mOffset += Length;
        }

        // -=(Undocumented)=-
        void WriteBool(Bool Data)
        {
            Write<Bool>(Data);
        }

        // -=(Undocumented)=-
        template<typename Type>
        void WriteInt(Type Data)
        {
            for (; Data > 0x7F; Data >>= 7u)
            {
                WriteUInt8(static_cast<UInt08>(Data & 0x7Fu) | 0x80u);
            }
            WriteUInt8(static_cast<UInt08>(Data) & 0x7Fu);
        }

        // -=(Undocumented)=-
        void WriteInt8(SInt08 Data)
        {
            Write<SInt08>(Data);
        }

        // -=(Undocumented)=-
        void WriteUInt8(UInt08 Data)
        {
            Write<UInt08>(Data);
        }

        // -=(Undocumented)=-
        void WriteInt16(SInt16 Data)
        {
            Write<SInt16>(Data);
        }

        // -=(Undocumented)=-
        void WriteUInt16(UInt16 Data)
        {
            Write<UInt16>(Data);
        }

        // -=(Undocumented)=-
        void WriteInt32(SInt32 Data)
        {
            Write<SInt32>(Data);
        }

        // -=(Undocumented)=-
        void WriteUInt32(UInt32 Data)
        {
            Write<UInt32>(Data);
        }

        // -=(Undocumented)=-
        void WriteInt64(SInt64 Data)
        {
            Write<SInt64>(Data);
        }

        // -=(Undocumented)=-
        void WriteUInt64(UInt64 Data)
        {
            Write<UInt64>(Data);
        }

        // -=(Undocumented)=-
        void WriteReal32(Real32 Data)
        {
            Write<Real32>(Data);
        }

        // -=(Undocumented)=-
        void WriteReal64(Real64 Data)
        {
            Write<Real64>(Data);
        }

        // -=(Undocumented)=-
        void WriteString8(CStr Data)
        {
            WriteInt(Data.size());

            if (!Data.empty())
            {
                Write<CStr::const_pointer>(Data.data(), Data.size() * sizeof(CStr::value_type));
            }
        }

        // -=(Undocumented)=-
        void WriteString16(CStr16 Data)
        {
            WriteInt(Data.size());

            if (!Data.empty())
            {
                Write<CStr16::const_pointer>(Data.data(), Data.size() * sizeof(CStr16::value_type));
            }
        }

        // -=(Undocumented)=-
        template<typename Object>
        void WriteObject(Ref<Object> Data)
        {
            Object::Write(* this, Data);
        }

        // -=(Undocumented)=-
        template<typename Type>
        void WriteNumber(Type Number)
        {
            if constexpr (std::is_integral_v<Type>)
            {
                WriteInt<Type>(Number);
            }
            else
            {
                Write<Type>(Number);
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<UInt08> mBuffer;
        UInt           mOffset;
    };
}
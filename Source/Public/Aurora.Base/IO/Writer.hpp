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
    class Writer final
    {
    public:

        // -=(Undocumented)=-
        static constexpr inline UInt32 kDefaultCapacity = 1024;

    public:

        // -=(Undocumented)=-
        explicit Writer(UInt Capacity = kDefaultCapacity)
            : mBuffer ( Capacity, 0 ),
              mOffset { 0 }
        {
        }

        // -=(Undocumented)=-
        CPtr<UInt8> GetData()
        {
            return CPtr<UInt8>(mBuffer.data(), mOffset);
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
                std::memcpy(& mBuffer[mOffset], Data, Length);
            }
            else
            {
                reinterpret_cast<Ref<Type>>(mBuffer[mOffset]) = Data;
            }
            mOffset += Length;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void WriteAny(Type Data)
        {
            if constexpr (std::is_arithmetic_v<Type>)
            {
                WriteNumber(Data);
            }
            else if constexpr (std::is_same<Type, SStr>::value || std::is_same<Type, CStr>::value)
            {
                WriteString8(Data);
            }
            else if constexpr (std::is_same<Type, SStr16>::value || std::is_same<Type, CStr16>::value)
            {
                WriteString16(Data);
            }
            else
            {
                WriteObject(Data);
            }
        }

        // -=(Undocumented)=-
        void WriteBool(Bool Data)
        {
            Write<Bool>(Data);
        }

        // -=(Undocumented)=-
        template<typename Type, typename = std::enable_if_t<std::is_integral<Type>::value>>
        void WriteInt(Type Data)
        {
            for (; Data > 0x7F; Data >>= 7u)
            {
                WriteUInt8(static_cast<UInt8>(Data & 0x7Fu) | 0x80u);
            }
            WriteUInt8(static_cast<UInt8>(Data) & 0x7Fu);
        }

        // -=(Undocumented)=-
        void WriteInt8(SInt8 Data)
        {
            Write<SInt8>(Data);
        }

        // -=(Undocumented)=-
        void WriteUInt8(UInt8 Data)
        {
            Write<UInt8>(Data);
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
        template<typename Type, typename = std::enable_if_t<std::is_arithmetic<Type>::value>>
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

        // -=(Undocumented)=-
        template<typename Type>
        void WriteObject(Ref<Type> Data)
        {
            Type::Write(* this, Data);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector<UInt8> mBuffer;
        UInt          mOffset;
    };
}
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

#include "Reader.hpp"
#include "Writer.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    template<typename T>
    class Stream
    {
    public:

        // -=(Undocumented)=-
        Stream(Ref<T> Archive)
            : mArchive { Archive }
        {
        }

        // -=(Undocumented)=-
        template<typename Data, UInt Size>
        void SerializeArray(Ref<Array<Data, Size>> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                const auto Chunk = mArchive.template Read<Ptr<Data>>(Size * sizeof(Data));

                std::copy(Chunk, Chunk + Size, Value.data());
            }
            else
            {
                mArchive.Write(Value.data(), Size * sizeof(Data));
            }
        }

        // -=(Undocumented)=-
        void SerializeBool(Ref<Bool> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = mArchive.ReadBool();
            }
            else
            {
                mArchive.WriteBool(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Int>
        void SerializeInt(Ref<Int> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = mArchive.template ReadInt<Int>();
            }
            else
            {
                mArchive.WriteInt(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Int>
        void SerializeInt8(Ref<Int> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = static_cast<Int>(mArchive.ReadInt8());
            }
            else
            {
                mArchive.WriteInt8(static_cast<SInt08>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Int>
        void SerializeUInt8(Ref<Int> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = static_cast<Int>(mArchive.ReadUInt8());
            }
            else
            {
                mArchive.WriteUInt8(static_cast<UInt08>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Int>
        void SerializeInt16(Ref<Int> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = static_cast<Int>(mArchive.ReadInt16());
            }
            else
            {
                mArchive.WriteInt16(static_cast<SInt16>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Int>
        void SerializeUInt16(Ref<Int> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = static_cast<Int>(mArchive.ReadUInt16());
            }
            else
            {
                mArchive.WriteUInt16(static_cast<UInt16>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Int>
        void SerializeInt32(Ref<Int> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = static_cast<Int>(mArchive.ReadInt32());
            }
            else
            {
                mArchive.WriteInt32(static_cast<SInt32>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Int>
        void SerializeUInt32(Ref<Int> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = static_cast<Int>(mArchive.ReadUInt32());
            }
            else
            {
                mArchive.WriteUInt32(static_cast<UInt32>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Int>
        void SerializeInt64(Ref<Int> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = static_cast<Int>(mArchive.ReadInt64());
            }
            else
            {
                mArchive.WriteInt64(static_cast<SInt64>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Int>
        void SerializeUInt64(Ref<Int> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = static_cast<Int>(mArchive.ReadUInt64());
            }
            else
            {
                mArchive.WriteUInt64(static_cast<UInt64>(Value));
            }
        }

        // -=(Undocumented)=-
        void SerializeReal32(Ref<Real32> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = mArchive.ReadReal32();
            }
            else
            {
                mArchive.WriteReal32(Value);
            }
        }

        // -=(Undocumented)=-
        void SerializeReal64(Ref<Real64> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = mArchive.ReadReal64();
            }
            else
            {
                mArchive.WriteReal64(Value);
            }
        }

        // -=(Undocumented)=-
        void SerializeString8(Ref<SStr> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = mArchive.ReadString8();
            }
            else
            {
                mArchive.WriteString8(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Enum>
        void SerializeEnum(Ref<Enum> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = static_cast<Enum>(mArchive.template ReadInt<SInt>());
            }
            else
            {
                mArchive.WriteInt(static_cast<SInt>(Value));
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        const Ref<T> mArchive;
    };
}
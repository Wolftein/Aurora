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
        static constexpr Bool k_Reader = std::is_same_v<T, Reader>;

        // -=(Undocumented)=-
        static constexpr Bool k_Writer = std::is_same_v<T, Writer>;

    public:

        // -=(Undocumented)=-
        Stream(Ref<T> Archive)
            : mArchive { Archive }
        {
        }

        // -=(Undocumented)=-
        void SerializeBool(Ref<Bool> Value)
        {
            if constexpr (k_Reader)
            {
                Value = mArchive.ReadBool();
            }
            else
            {
                mArchive.WriteBool(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeEnum(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = mArchive.template ReadEnum<Type>();
            }
            else
            {
                mArchive.template WriteEnum<Type>(static_cast<Type>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeInt(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = mArchive.template ReadInt<Type>();
            }
            else
            {
                mArchive.WriteInt(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeInt8(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = static_cast<Type>(mArchive.ReadInt8());
            }
            else
            {
                mArchive.WriteInt8(static_cast<SInt8>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeUInt8(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = static_cast<Type>(mArchive.ReadUInt8());
            }
            else
            {
                mArchive.WriteUInt8(static_cast<UInt8>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeInt16(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = static_cast<Type>(mArchive.ReadInt16());
            }
            else
            {
                mArchive.WriteInt16(static_cast<SInt16>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeUInt16(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = static_cast<Type>(mArchive.ReadUInt16());
            }
            else
            {
                mArchive.WriteUInt16(static_cast<UInt16>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeInt32(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = static_cast<Type>(mArchive.ReadInt32());
            }
            else
            {
                mArchive.WriteInt32(static_cast<SInt32>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeUInt32(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = static_cast<Type>(mArchive.ReadUInt32());
            }
            else
            {
                mArchive.WriteUInt32(static_cast<UInt32>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeInt64(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = static_cast<Type>(mArchive.ReadInt64());
            }
            else
            {
                mArchive.WriteInt64(static_cast<SInt64>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeUInt64(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = static_cast<Type>(mArchive.ReadUInt64());
            }
            else
            {
                mArchive.WriteUInt64(static_cast<UInt64>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeReal32(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = static_cast<Type>(mArchive.ReadReal32());
            }
            else
            {
                mArchive.WriteReal32(static_cast<Real32>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeReal64(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = static_cast<Type>(mArchive.ReadReal64());
            }
            else
            {
                mArchive.WriteReal32(static_cast<Real64>(Value));
            }
        }

        // -=(Undocumented)=-
        void SerializeString8(Ref<SStr> Value)
        {
            if constexpr (k_Reader)
            {
                Value = mArchive.ReadString8();
            }
            else
            {
                mArchive.WriteString8(Value);
            }
        }

        // -=(Undocumented)=-
        void SerializeString16(Ref<SStr16> Value)
        {
            if constexpr (k_Reader)
            {
                Value = mArchive.ReadString16();
            }
            else
            {
                mArchive.WriteString16(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeNumber(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = mArchive.template ReadNumber<Type>();
            }
            else
            {
                mArchive.template WriteNumber<Type>(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeObject(Ref<Type> Value)
        {
            if constexpr (k_Reader)
            {
                Value = mArchive.template ReadObject<Type>(); // @TODO: In-place serialization
            }
            else
            {
                mArchive.template WriteObject<Type>(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SerializeVector(Ref<Vector<Type>> Value)
        {
            if constexpr (k_Reader)
            {
                mArchive.template ReadVector<Type>(Value);
            }
            else
            {
                mArchive.template WriteVector<Type>(Value);
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ref<T> mArchive;
    };
}
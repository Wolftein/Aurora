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

inline namespace IO
{
    // -=(Undocumented)=-
    template<typename T>
    class Stream
    {
    public:

        // -=(Undocumented)=-
        static constexpr Bool k_Reader = std::is_same<T, Reader>::value;

        // -=(Undocumented)=-
        static constexpr Bool k_Writer = std::is_same<T, Writer>::value;

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
        template<typename Int>
        void SerializeInt(Ref<Int> Value)
        {
            if constexpr (k_Reader)
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
            if constexpr (k_Reader)
            {
                Value = static_cast<Int>(mArchive.ReadInt8());
            }
            else
            {
                mArchive.WriteInt8(static_cast<SInt8>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Int>
        void SerializeUInt8(Ref<Int> Value)
        {
            if constexpr (k_Reader)
            {
                Value = static_cast<Int>(mArchive.ReadUInt8());
            }
            else
            {
                mArchive.WriteUInt8(static_cast<UInt8>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Int>
        void SerializeInt16(Ref<Int> Value)
        {
            if constexpr (k_Reader)
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
            if constexpr (k_Reader)
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
            if constexpr (k_Reader)
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
            if constexpr (k_Reader)
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
            if constexpr (k_Reader)
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
            if constexpr (k_Reader)
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
            if constexpr (k_Reader)
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
            if constexpr (k_Reader)
            {
                Value = mArchive.ReadReal64();
            }
            else
            {
                mArchive.WriteReal64(Value);
            }
        }

        // -=(Undocumented)=-
        void SerializeString(Ref<SStr> Value)
        {
            if constexpr (k_Reader)
            {
                Value = mArchive.ReadString();
            }
            else
            {
                mArchive.WriteString(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Enum>
        void SerializeEnum(Ref<Enum> Value)
        {
            using Type = std::underlying_type_t<Enum>;

            if constexpr (k_Reader)
            {
                Value = static_cast<Enum>(mArchive.template ReadInt<Type>());
            }
            else
            {
                mArchive.WriteInt(static_cast<Type>(Value));
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
                Value = mArchive.template ReadObject<Type>();
            }
            else
            {
                mArchive.template WriteObject<Type>(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Type, UInt Capacity>
        void SerializeArray(Ref<Array<Type, Capacity>> Value)
        {
            if constexpr (k_Reader)
            {
                for (Ref<Type> Element : Value)
                {
                    Element = mArchive.template ReadAny<Type>();
                }
            }
            else
            {
                for (Ref<Type> Element : Value)
                {
                    Element = mArchive.template WriteAny<Type>(Element);
                }
            }
        }

        // -=(Undocumented)=-
        template<typename Type, UInt Capacity = UINT16_MAX>
        void SerializeList(Ref<Vector<Type>> Value)
        {
            if constexpr (k_Reader)
            {
                const UInt Length = mArchive.template ReadInt<UInt>();
                if (Length >= Capacity)
                {
                    return;
                }

                Value.clear();
                Value.reserve(Length);

                for (UInt Element = 0; Element < Length; ++Element)
                {
                    Value.emplace_back(mArchive.template ReadAny<Type>());
                }
            }
            else
            {
                mArchive.template WriteInt<UInt>(Value.size());

                for (Ref<Type> Element : Value)
                {
                    mArchive.template WriteAny<Type>(Element);
                }
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ref<T> mArchive;
    };
}
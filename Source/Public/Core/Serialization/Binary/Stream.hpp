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
        static constexpr Bool kReader = std::is_same<T, Reader>::value;

        // -=(Undocumented)=-
        static constexpr Bool kWriter = std::is_same<T, Writer>::value;

    public:

        // -=(Undocumented)=-
        Stream(Ref<T> Archive)
            : mArchive { Archive }
        {
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
        void SerializeString16(Ref<SStr16> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = mArchive.ReadString16();
            }
            else
            {
                mArchive.WriteString16(Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Enum>
        void SerializeEnum(Ref<Enum> Value)
        {
            using Type = std::underlying_type_t<Enum>;

            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = static_cast<Enum>(mArchive.template ReadInt<Type>());
            }
            else
            {
                mArchive.WriteInt(static_cast<Type>(Value));
            }
        }

        // -=(Undocumented)=-
        template<typename Object>
        void SerializeObject(Ref<Object> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
            {
                Value = Object::Read(mArchive);
            }
            else
            {
                Object::Write(mArchive, Value);
            }
        }

        // -=(Undocumented)=-
        template<typename Object, UInt Capacity = UINT32_MAX>
        void SerializeList(Ref<Vector<Object>> Value)
        {
            if constexpr (std::is_same<T, Reader>::value)
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
                    Value.emplace_back(Object::Read(mArchive));
                }
            }
            else
            {
                mArchive.template WriteInt<UInt>(Value.size());

                for (Ref<const Object> Element : Value)
                {
                    Object::Write(mArchive, Element);
                }
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ref<T> mArchive;
    };
}
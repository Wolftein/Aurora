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

inline namespace Base
{
    /// \brief Generic serialization archive for reading or writing binary data.
    /// 
    /// provides a unified interface for serializing data to or from a binary stream,
    /// depending on whether the underlying archive type is a `Reader` or a `Writer`.
    /// 
    /// \tparam T The underlying archive type. Must be either `Reader` or `Writer`.
    template<typename T>
    class Archive
    {
    public:

        /// \brief Whether the archive is a `Reader`.
        static constexpr Bool IsReader = IsEqual<T, Reader>;

        /// \brief Whether the archive is a `Writer`.
        static constexpr Bool IsWriter = IsEqual<T, Writer>;

    public:

        /// \brief Constructs an archive wrapper from an underlying reader or writer.
        /// 
        /// \param Archive Reference to the internal reader or writer instance.
        AURORA_INLINE Archive(Ref<T> Archive)
            : mArchive { Archive }
        {
        }

        /// \brief Serializes a boolean value.
        /// 
        /// \param Value Reference to the boolean value to serialize.
        AURORA_INLINE void SerializeBool(Ref<Bool> Value)
        {
            if constexpr (IsReader)
            {
                Value = mArchive.ReadBool();
            }
            else
            {
                mArchive.WriteBool(Value);
            }
        }

        /// \brief Serializes an enum value.
        /// 
        /// \tparam Type The enum type to serialize.
        /// \param Value Reference to the enum value.
        template<typename Type>
        AURORA_INLINE void SerializeEnum(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = mArchive.template ReadEnum<Type>();
            }
            else
            {
                mArchive.template WriteEnum<Type>(static_cast<Type>(Value));
            }
        }

        /// \brief Serializes an integer value using platform-native size and format.
        /// 
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeInt(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = mArchive.template ReadInt<Type>();
            }
            else
            {
                mArchive.WriteInt(Value);
            }
        }

        /// \brief Serializes an 8-bit signed integer.
        /// 
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeInt8(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = static_cast<Type>(mArchive.ReadInt8());
            }
            else
            {
                mArchive.WriteInt8(static_cast<SInt8>(Value));
            }
        }

        /// \brief Serializes an 8-bit unsigned integer.
        /// 
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeUInt8(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = static_cast<Type>(mArchive.ReadUInt8());
            }
            else
            {
                mArchive.WriteUInt8(static_cast<UInt8>(Value));
            }
        }

        /// \brief Serializes a 16-bit signed integer.
        /// 
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeInt16(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = static_cast<Type>(mArchive.ReadInt16());
            }
            else
            {
                mArchive.WriteInt16(static_cast<SInt16>(Value));
            }
        }

        /// \brief Serializes a 16-bit unsigned integer.
        /// 
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeUInt16(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = static_cast<Type>(mArchive.ReadUInt16());
            }
            else
            {
                mArchive.WriteUInt16(static_cast<UInt16>(Value));
            }
        }

        /// \brief Serializes a 32-bit signed integer.
        /// 
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeInt32(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = static_cast<Type>(mArchive.ReadInt32());
            }
            else
            {
                mArchive.WriteInt32(static_cast<SInt32>(Value));
            }
        }

        /// \brief Serializes a 32-bit unsigned integer.
        /// 
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeUInt32(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = static_cast<Type>(mArchive.ReadUInt32());
            }
            else
            {
                mArchive.WriteUInt32(static_cast<UInt32>(Value));
            }
        }

        /// \brief Serializes a 64-bit signed integer.
        /// 
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeInt64(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = static_cast<Type>(mArchive.ReadInt64());
            }
            else
            {
                mArchive.WriteInt64(static_cast<SInt64>(Value));
            }
        }

        /// \brief Serializes a 64-bit unsigned integer.
        /// 
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeUInt64(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = static_cast<Type>(mArchive.ReadUInt64());
            }
            else
            {
                mArchive.WriteUInt64(static_cast<UInt64>(Value));
            }
        }

        /// \brief Serializes a 32-bit floating point value.
        /// 
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeReal32(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = static_cast<Type>(mArchive.ReadReal32());
            }
            else
            {
                mArchive.WriteReal32(static_cast<Real32>(Value));
            }
        }

        /// \brief Serializes a 64-bit floating point value.
        /// 
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeReal64(Ref<Type> Value)
        {
            if constexpr (IsReader)
            {
                Value = static_cast<Type>(mArchive.ReadReal64());
            }
            else
            {
                mArchive.WriteReal64(static_cast<Real64>(Value));
            }
        }

        /// \brief Serializes a floating point value.
        ///
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeReal(Ref<Type> Value)
        {
            if constexpr(IsEqual<Type, Real32>)
            {
                SerializeReal32(Value);
            }
            else
            {
                SerializeReal64(Value);
            }
        }

        /// \brief Serializes a text string.
        /// 
        /// \param Value Reference to the value.
        AURORA_INLINE void SerializeText(Ref<Text> Value)
        {
            if constexpr (IsReader)
            {
                Value = mArchive.ReadString();
            }
            else
            {
                mArchive.WriteString(Value);
            }
        }

        /// \brief Serializes an object.
        ///
        /// \param Value Reference to the value.
        template<typename Type>
        AURORA_INLINE void SerializeObject(Ref<Type> Value)
        {
            if constexpr(IsBool<Type>)
            {
                SerializeBool(Value);
            }
            else if constexpr(IsEnum<Type>)
            {
                SerializeEnum(Value);
            }
            else if constexpr(IsInteger<Type>)
            {
                SerializeInt(Value);
            }
            else if constexpr(IsReal<Type>)
            {
                SerializeReal(Value);
            }
            else if constexpr(IsText<Type>)
            {
                SerializeText(Value);
            }
            else
            {
                Value.OnSerialize(* this);
            }
        }

        /// \brief Serializes an array.
        ///
        /// \param Array Reference to the array.
        template<typename Type>
        AURORA_INLINE void SerializeArray(Ref<Type> Array)
        {
            for (UInt32 Element = 0, Size = Array.size(); Element < Size; ++Element)
            {
                SerializeObject(Array[Element]);
            }
        }

        /// \brief Serializes a vector.
        ///
        /// \param Vector Reference to the vector.
        template<typename Type>
        AURORA_INLINE void SerializeVector(Ref<Type> Vector)
        {
            if constexpr (IsReader)
            {
                Vector.resize(mArchive.template ReadInt<UInt32>());
            }
            else
            {
                mArchive.template WriteInt<UInt32>(Vector.size());
            }

            for (UInt32 Element = 0, Size = Vector.size(); Element < Size; ++Element)
            {
                SerializeObject(Vector[Element]);
            }
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Ref<T> mArchive;
    };
}
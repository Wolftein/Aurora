// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Core/Core.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
	// -=(Undocumented)=-
	class Reader final
	{
	public:

		// -=(Undocumented)=-
		Reader(UPtr<UInt08[]> Data, UInt32 Capacity)
			: mData   { eastl::move(Data) },
			  mLength { Capacity },
			  mOffset { 0 }
		{
		}

		// -=(Undocumented)=-
		Reader(UInt Capacity)
			: mData   { eastl::make_unique<UInt08[]>(Capacity) },
			  mLength { Capacity },
			  mOffset { 0 }
		{
		}

        // -=(Undocumented)=-
        auto GetData()
        {
            return mData.get();
        }

		// -=(Undocumented)=-
		UInt GetCapacity() const
		{
			return mLength;
		}

		// -=(Undocumented)=-
        UInt GetRemaining() const
		{
			return mLength - mOffset;
		}

		// -=(Undocumented)=-
		Bool HasData()
		{
			return GetCapacity() > 0;
		}

		// -=(Undocumented)=-
		Bool HasRemaining()
		{
			return GetRemaining() > 0;
		}

		// -=(Undocumented)=-
		template<typename Type>
		Type Peek(UInt Length = sizeof(Type))
		{
			if (mOffset + Length > mLength)
			{
				return { /* 0 */ };
			}

			if constexpr (eastl::is_pointer_v<Type>)
			{
				return reinterpret_cast<Type>(mData[mOffset]);
			}
			else
			{
				return * reinterpret_cast<Ptr<Type>>(mData[mOffset]);
			}
		}

		// -=(Undocumented)=-
		template<typename Type>
		Type Read(UInt Length = sizeof(Type))
		{
			const Type zResult = Peek<Type>(Length);
			mOffset += Length;
			return zResult;
		}

		// -=(Undocumented)=-
		SInt08 ReadInt8()
		{
			return Read<SInt08>();
		}

		// -=(Undocumented)=-
		UInt08 ReadUnsignedInt8()
		{
			return Read<UInt08>();
		}

		// -=(Undocumented)=-
		SInt16 ReadInt16()
		{
			return Read<SInt16>();
		}

		// -=(Undocumented)=-
		UInt16 ReadUnsignedInt16()
		{
			return Read<UInt16>();
		}

		// -=(Undocumented)=-
		SInt32 ReadInt32()
		{
			return Read<SInt32>();
		}

		// -=(Undocumented)=-
		UInt32 ReadUnsignedInt32()
		{
			return Read<UInt32>();
		}

		// -=(Undocumented)=-
		SInt64 ReadInt64()
		{
			return Read<SInt64>();
		}

		// -=(Undocumented)=-
		UInt64 ReadUnsignedInt64()
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

	private:

		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
		// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

		UPtr<UInt08[]> mData;
		UInt           mLength;
        UInt           mOffset;
	};
}
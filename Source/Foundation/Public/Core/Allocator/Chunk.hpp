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

#include "Core/Common.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    class Chunk final
    {
    public:

        // -=(Undocumented)=-
        Chunk() : Chunk(nullptr, 0)
        {
        }

        // -=(Undocumented)=-
        Chunk(UPtr<UInt08[]> Data, UInt32 Capacity)
            : mData   { eastl::move(Data) },
              mLength { Capacity }
        {
        }

        // -=(Undocumented)=-
        Chunk(UInt Capacity)
            : mData   { eastl::make_unique<UInt08[]>(Capacity) },
              mLength { Capacity }
        {
        }

        // -=(Undocumented)=-
        auto GetData()
        {
            return mData.get();
        }

        // -=(Undocumented)=-
        Bool HasData() const
        {
            return GetSize() > 0;
        }

        // -=(Undocumented)=-
        UInt GetSize() const
        {
            return mLength;
        }

        // -=(Undocumented)=-
        void Clear()
        {
            mData   = nullptr;
            mLength = 0;
        }

        // -=(Undocumented)=-
        template<typename T>
        operator CPtr<T>() const
        {
            return CPtr<T> { reinterpret_cast<Ptr<T>>(mData.get()), mLength / sizeof(T) };
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        UPtr<UInt08[]> mData;
        UInt           mLength;
    };
}
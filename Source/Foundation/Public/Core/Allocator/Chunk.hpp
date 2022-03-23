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
        using Deleter = void(*)(Ptr<void>);

        // -=(Undocumented)=-
        using Pointer = UPtr<void, Deleter>;

    public:

        // -=(Undocumented)=-
        Chunk()
            : mData { nullptr, nullptr },
              mSize { 0 }
        {
        }

        // -=(Undocumented)=-
        Chunk(UInt Size)
            : mData { new UInt08[Size], [](auto Data) { delete[] reinterpret_cast<Ptr<UInt08>>(Data); } },
              mSize { Size }
        {
        }

        // -=(Undocumented)=-
        Chunk(Pointer Data, UInt Size)
            : mData { eastl::move(Data) },
              mSize { Size }
        {
        }

        // -=(Undocumented)=-
        template<typename Type>
        Chunk(Type Data, UInt Size, Deleter Destructor)
            : mData { reinterpret_cast<Ptr<UInt08>>(Data), Destructor },
              mSize { Size }
        {
        }

        // -=(Undocumented)=-
        template<typename T = void>
        auto GetData()
        {
            return reinterpret_cast<Ptr<T>>(mData.get());
        }

        // -=(Undocumented)=-
        template<typename T>
        auto GetSpan() const
        {
            return CPtr<T> { reinterpret_cast<Ptr<T>>(mData.get()), mSize / sizeof(T) };
        }

        // -=(Undocumented)=-
        Bool HasData() const
        {
            return GetSize() > 0;
        }

        // -=(Undocumented)=-
        UInt GetSize() const
        {
            return mSize;
        }

        // -=(Undocumented)=-
        void Clear()
        {
            mData = nullptr;
            mSize = 0;
        }

        // -=(Undocumented)=-
        template<typename T>
        operator CPtr<T>() const
        {
            return GetSpan<T>();
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Pointer mData;
        UInt    mSize;
    };
}
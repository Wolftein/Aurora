// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
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
            : mData { Move(Data) },
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
        Chunk(Chunk && Other)
            : mData { Move(Other.mData) },
              mSize { Exchange(Other.mSize, 0) }
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
        auto GetText() const
        {
            return CStr { reinterpret_cast<CStr::pointer>(mData.get()), mSize };
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
        Ref<Chunk> operator=(Chunk && Other)
        {
            mData = Move(Other.mData);
            mSize = Exchange(Other.mSize, 0);

            return (* this);
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
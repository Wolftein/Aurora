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

#include <Core/Math/Rect.hpp>
#include <Core/Math/Vector2.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Core
{
    // -=(Undocumented)=-
    class Packer
    {
    public:

        // -=(Undocumented)=-
        Packer(UInt Width, UInt Height);

        // -=(Undocumented)=-
        template<typename Type>
        Packer(Vector2<Type> Size)
            : Packer(Size.GetX(), Size.GetY())
        {
        }

        // -=(Undocumented)=-
        Vector2i GetDimension() const
        {
            return mDimension;
        }

        // -=(Undocumented)=-
        void Reset(UInt Width, UInt Height);

        // -=(Undocumented)=-
        template<typename Type>
        void Reset(Vector2<Type> Size)
        {
            Reset(Size.GetX(), Size.GetY());
        }

        // -=(Undocumented)=-
        Recti Add(UInt Width, UInt Height);

        // -=(Undocumented)=-
        template<typename Type>
        Recti Add(Vector2<Type> Size)
        {
            return Add(Size.GetX(), Size.GetY());
        }

        // -=(Undocumented)=-
        void Remove(Ref<const Recti> Rect);

    private:

        // -=(Undocumented)=-
        struct Rectangle
        {
            UInt X;
            UInt Y;
            UInt Width;
            UInt Height;

            // -=(Undocumented)=-
            Rectangle(UInt X, UInt Y, UInt Width, UInt Height) : X { X }, Y { Y }, Width { Width }, Height { Height }
            {
            }
        };

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Vector2i          mDimension;
        Vector<Rectangle> mSpaces;
    };
}
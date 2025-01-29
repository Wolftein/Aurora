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

#include "Rect.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    class Pivot final
    {
    public:

        // -=(Undocumented)=-
        enum class Type : UInt8
        {
            // -=(Undocumented)=-
            LeftTop,

            // -=(Undocumented)=-
            LeftMiddle,

            // -=(Undocumented)=-
            LeftBottom,

            // -=(Undocumented)=-
            LeftBaseline,

            // -=(Undocumented)=-
            CenterTop,

            // -=(Undocumented)=-
            CenterMiddle,

            // -=(Undocumented)=-
            CenterBottom,

            // -=(Undocumented)=-
            CenterBaseline,

            // -=(Undocumented)=-
            RightTop,

            // -=(Undocumented)=-
            RightMiddle,

            // -=(Undocumented)=-
            RightBottom,

            // -=(Undocumented)=-
            RightBaseline,
        };

    public:

        // -=(Undocumented)=-
        explicit Pivot(Type Type = Type::LeftTop)
            : mType { Type }
        {
        }

        // -=(Undocumented)=-
        Type GetType() const
        {
            return mType;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeEnum(mType);
        }

    public:

        // -=(Undocumented)=-
        template<typename Base>
        Rect<Base> Calculate(ConstRef<Vector2<Base>> Position, ConstRef<Vector2<Base>> Size) const
        {
            static constexpr Rect<Base> k_Multiplier[] = {
                Rect<Base>( 0.0,  0.0, 1.0, 1.0),  // TopLeft
                Rect<Base>(-0.5,  0.0, 0.5, 1.0),  // TopCenter
                Rect<Base>(-1.0,  0.0, 0.0, 1.0),  // TopRight
                Rect<Base>( 0.0, -0.5, 1.0, 0.5),  // MiddleLet
                Rect<Base>(-0.5, -0.5, 0.5, 0.5),  // MiddleCenter
                Rect<Base>(-1.0, -0.5, 0.0, 0.5),  // MiddleRight
                Rect<Base>( 0.0, -1.0, 1.0, 0.0),  // BottomLet
                Rect<Base>(-0.5, -1.0, 0.5, 0.0),  // BottomCenter
                Rect<Base>(-1.0, -1.0, 0.0, 0.0),  // BottomRight
            };
            return k_Multiplier[CastEnum(mType)] * Size + Position;
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Type mType;
    };
}
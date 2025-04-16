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

#include "Circle.hpp"
#include "Edge.hpp"
#include "Rect.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{
    // -=(Undocumented)=-
    template<typename Base>
    class Shape final
    {
    public:

        // -=(Undocumented)=-
        enum class Kind : UInt8
        {
            // -=(Undocumented)=-
            Circle,

            // -=(Undocumented)=-
            Line,

            // -=(Undocumented)=-
            Rectangle,

            // -=(Undocumented)=-
            None,
        };

    public:

        // -=(Undocumented)=-
        constexpr Shape()
            : mKind { Kind::None }
        {
        }

        // -=(Undocumented)=-
        constexpr Shape(ConstRef<Circle<Base>> Circle)
            : mKind { Kind::Circle }
        {
            mData.Circle = Circle;
        }

        // -=(Undocumented)=-
        constexpr Shape(ConstRef<Edge<Base>> Line)
            : mKind { Kind::Line }
        {
            mData.Line = Line;
        }

        // -=(Undocumented)=-
        constexpr Shape(ConstRef<Rect<Base>> Rectangle)
            : mKind { Kind::Rectangle }
        {
            mData.Rectangle = Rectangle;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void SetData(ConstRef<Type> Data)
        {
            if      constexpr (std::is_same_v<Circlef, Type>)
            {
                mKind           = Kind::Circle;
                mData.Circle    = Data;
            }
            else if constexpr (std::is_same_v<Edgef,   Type>)
            {
                mKind           = Kind::Line;
                mData.Line      = Data;
            }
            else if constexpr (std::is_same_v<Rectf,   Type>)
            {
                mKind           = Kind::Rectangle;
                mData.Rectangle = Data;
            }
        }

        // -=(Undocumented)=-
        template<typename Type>
        ConstRef<Type> GetData() const
        {
            if      constexpr (std::is_same_v<Circlef, Type>)
            {
                return mData.Circle;
            }
            else if constexpr (std::is_same_v<Edgef,   Type>)
            {
                return mData.Line;
            }
            else if constexpr (std::is_same_v<Rectf,   Type>)
            {
                return mData.Rectangle;
            }
        }

        // -=(Undocumented)=-
        Kind GetKind() const
        {
            return mKind;
        }

        // -=(Undocumented)=-
        template<typename Type>
        void OnSerialize(Stream<Type> Archive)
        {
            Archive.SerializeEnum(mKind);

            switch (mKind)
            {
            case Kind::Circle:
                Archive.SerializeObject(mData.Circle);
                break;
            case Kind::Line:
                Archive.SerializeObject(mData.Line);
                break;
            case Kind::Rectangle:
                Archive.SerializeObject(mData.Rectangle);
                break;
            }
        }

    private:

        // -=(Undocumented)=-
        union Data
        {
            // -=(Undocumented)=-
            constexpr Data()
            {

            }

            // -=(Undocumented)=-
            Circle<Base> Circle;

            // -=(Undocumented)=-
            Edge<Base>   Line;

            // -=(Undocumented)=-
            Rect<Base>   Rectangle;
        };

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Data mData;
        Kind mKind;
    };

    // -=(Undocumented)=-
    using Shapef = Shape<Real32>;
}
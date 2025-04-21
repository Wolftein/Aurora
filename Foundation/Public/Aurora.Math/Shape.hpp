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

#include "Collision.hpp"

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
        };

    public:

        // -=(Undocumented)=-
        constexpr Shape()
            : mKind { Kind::Rectangle }
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
        Bool Intersects(ConstRef<Shape<Base>> Other, Ptr<Math::Manifold<Base>> Manifold) const
        {
            using Function = Bool(*)(ConstPtr<void>, ConstPtr<void>, Ptr<Math::Manifold<Base>>);

            static constexpr Function k_Dispatch[3][3] =
            {
                {
                    Intersects<Base, Circle<Base>, Circle<Base>>,
                    Intersects<Base, Circle<Base>, Edge<Base>>,
                    Intersects<Base, Circle<Base>, Rect<Base>>,
                },
                {
                    Intersects<Base, Edge<Base>, Circle<Base>>,
                    Intersects<Base, Edge<Base>, Edge<Base>>,
                    Intersects<Base, Edge<Base>, Rect<Base>>,
                },
                {
                    Intersects<Base, Rect<Base>, Circle<Base>>,
                    Intersects<Base, Rect<Base>, Edge<Base>>,
                    Intersects<Base, Rect<Base>, Rect<Base>>,
                }
           };

            const UInt32 Row    = CastEnum(GetKind());
            const UInt32 Column = CastEnum(Other.GetKind());
            return k_Dispatch[Row][Column](this, AddressOf(Other), Manifold);
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
            default:
                break;
            }
        }

    public:

        // -=(Undocumented)=-
        static Shape<Base> Transform(ConstRef<Shape<Base>> Object, Pivot Pivot)
        {
            switch(Object.GetKind())
            {
            case Kind::Circle:
                return Shape<Base>(Circle<Base>::Transform(Object.GetData<Circle<Base>>(), Pivot));
            case Kind::Line:
                return Shape<Base>(Edge<Base>::Transform(Object.GetData<Edge<Base>>(), Pivot));
            case Kind::Rectangle:
                return Shape<Base>(Rect<Base>::Transform(Object.GetData<Rect<Base>>(), Pivot));
            default:
                return Object;
            }
        }

        // -=(Undocumented)=-
        static Shape<Base> Transform(ConstRef<Shape<Base>> Object, ConstRef<Matrix4<Base>> Transform)
        {
            switch(Object.GetKind())
            {
            case Kind::Circle:
                return Shape<Base>(Circle<Base>::Transform(Object.GetData<Circle<Base>>(), Transform));
            case Kind::Line:
                return Shape<Base>(Edge<Base>::Transform(Object.GetData<Edge<Base>>(), Transform));
            case Kind::Rectangle:
                return Shape<Base>(Rect<Base>::Transform(Object.GetData<Rect<Base>>(), Transform));
            default:
                return Object;
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

        // -=(Undocumented)=-
        template<typename Base, typename Type, typename Other>
        static constexpr Bool Intersects(ConstPtr<void> First, ConstPtr<void> Second, Ptr<Manifold<Base>> Manifold)
        {
            return Math::Intersects<Base>(* static_cast<ConstPtr<Type>>(First), * static_cast<ConstPtr<Other>>(Second), Manifold);
        }

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Data mData;
        Kind mKind;
    };

    // -=(Undocumented)=-
    using Shapef = Shape<Real32>;
}
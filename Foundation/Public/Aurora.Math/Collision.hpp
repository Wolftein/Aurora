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
    static constexpr Bool Intersects(ConstRef<Math::Edge<Base>> Edge, ConstRef<Math::Circle<Base>> Circle)
    {
        // Early out if either point is inside
        const Base RadiusPow2 = Circle.GetRadius() * Circle.GetRadius();

        if ((Edge.GetPointA() - Circle.GetCenter()).GetLengthSquared() <= RadiusPow2
         || (Edge.GetPointB() - Circle.GetCenter()).GetLengthSquared() <= RadiusPow2)
        {
            return true;
        }

        const Vector2<Base> AB = Edge.GetPointB() - Edge.GetPointA();
        const Vector2<Base> AC = Circle.GetCenter() - Edge.GetPointA();

        const Base T = Core::Clamp(AC.Dot(AB) / AB.LengthSquared(), Base(0), Base(1));
        const Vector2<Base> Closest = Edge.GetPointA() + AB * T;

        const Vector2<Base> Delta = Circle.GetCenter() - Closest;
        return Delta.GetLengthSquared() <= Circle.GetRadius() * Circle.GetRadius();
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Edge<Base>> Edge, ConstRef<Math::Rect<Base>> Rectangle)
    {
        if (Rectangle.Contains(Edge.GetPointA()) || Rectangle.Contains(Edge.GetPointB()))
        {
            return true;
        }

        const Vector2<Base> TL = Rectangle.GetTopLeft();
        const Vector2<Base> TR = Rectangle.GetTopRight();
        const Vector2<Base> BL = Rectangle.GetBottomLeft();
        const Vector2<Base> BR = Rectangle.GetBottomRight();

        return Math::Intersects(Edge, Math::Edge<Base>(TL, TR))
            || Math::Intersects(Edge, Math::Edge<Base>(TR, BR))
            || Math::Intersects(Edge, Math::Edge<Base>(BR, BL))
            || Math::Intersects(Edge, Math::Edge<Base>(BL, TL));
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Circle<Base>> Circle, ConstRef<Math::Rect<Base>> Rectangle)
    {
        ConstRef<Vector2<Base>> Center = Circle.GetCenter();

        const Vector2<Base> Closest(
            Core::Clamp(Center.GetX(), Rectangle.GetLeft(), Rectangle.GetRight()),
            Core::Clamp(Center.GetY(), Rectangle.GetTop(),  Rectangle.GetBottom()));
        return (Center - Closest).GetLengthSquared() <= Circle.GetRadius() * Circle.GetRadius();
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Circle<Base>> Circle, ConstRef<Math::Edge<Base>> Edge)
    {
        return Intersects<Base>(Edge, Circle);
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Rect<Base>> Rectangle, ConstRef<Math::Circle<Base>> Circle)
    {
        return Intersects<Base>(Circle, Rectangle);
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Rect<Base>> Rectangle, ConstRef<Math::Edge<Base>> Edge)
    {
        return Intersects<Base>(Edge, Rectangle);
    }
}
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

#include "Shape.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Math
{

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Circle<Base>> Circle, ConstRef<Math::Circle<Base>> Other, Ptr<Math::Manifold<Base>> Manifold)
    {
        return Circle.Intersects(Other, Manifold);
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Circle<Base>> Circle, ConstRef<Math::Rect<Base>> Rectangle, Ptr<Math::Manifold<Base>> Manifold)
    {
        ConstRef<Vector2<Base>> Center = Circle.GetCenter();
        const Vector2<Base>     Closest(
            Core::Clamp(Center.GetX(), Rectangle.GetLeft(), Rectangle.GetRight()),
            Core::Clamp(Center.GetY(), Rectangle.GetTop(),  Rectangle.GetBottom()));

        const Vector2<Base> Delta = Center - Closest;
        const Base SqDistance     = Delta.GetLengthSquared();
        const Base SqRadius       = Circle.GetRadius() * Circle.GetRadius();

        if (SqDistance <= SqRadius)
        {
            if (Manifold)
            {
                Manifold->SetPenetration(Circle.GetRadius() - Sqrt(SqDistance));
                Manifold->SetNormal(Vector2<Base>::Normalize(Delta));
                Manifold->AddPoint(Closest);
            }
            return true;
        }
        return false;
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Circle<Base>> Circle, ConstRef<Math::Edge<Base>> Edge, Ptr<Math::Manifold<Base>> Manifold)
    {
        const Vector2<Base> AB = Edge.GetPointB() - Edge.GetPointA();
        const Vector2<Base> AC = Circle.GetCenter() - Edge.GetPointA();

        const Base T                = Core::Clamp(AC.Dot(AB) / AB.GetLengthSquared(), Base(0), Base(1));
        const Vector2<Base> Closest = Edge.GetPointA() + AB * T;

        const Vector2<Base> Delta   = Circle.GetCenter() - Closest;
        const Base SqDistance       = Delta.GetLengthSquared();
        const Base SqRadius         = Circle.GetRadius() * Circle.GetRadius();

        if (SqDistance <= SqRadius)
        {
            if (Manifold)
            {
                const Base Distance        = Sqrt(SqDistance);
                const Vector2<Base> Normal = (Distance > k_Epsilon<Base>)
                    ? Delta / Distance
                    : Vector2<Base>::Normalize(Vector2<Base>(-AB.GetY(), AB.GetX()));

                Manifold->SetPenetration(Circle.GetRadius() - Distance);
                Manifold->SetNormal(Normal);
                Manifold->AddPoint(Closest);
            }
            return true;
        }
        return false;
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Rect<Base>> Rectangle, ConstRef<Math::Rect<Base>> Other, Ptr<Math::Manifold<Base>> Manifold)
    {
        return Rectangle.Intersects(Other, Manifold);
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Rect<Base>> Rectangle, ConstRef<Math::Circle<Base>> Circle, Ptr<Math::Manifold<Base>> Manifold)
    {
        return Intersects<Base>(Circle, Rectangle, Manifold);
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Rect<Base>> Rectangle, ConstRef<Math::Edge<Base>> Edge, Ptr<Math::Manifold<Base>> Manifold)
    {
        const Vector2<Base> Delta = Edge.GetPointB() - Edge.GetPointA();

        Base T0 = 0.0;
        Base T1 = 1.0;

        if (Abs(Delta.GetX()) < k_Epsilon<Base>)
        {
            if (Edge.GetPointA().GetX() < Rectangle.GetLeft() || Edge.GetPointA().GetX() > Rectangle.GetRight())
            {
                return false;
            }
        }
        else
        {
            const Base InvDelta = 1.0 / Delta.GetX();
            Base Near           = (Rectangle.GetLeft()  - Edge.GetPointA().GetX()) * InvDelta;
            Base Far            = (Rectangle.GetRight() - Edge.GetPointA().GetX()) * InvDelta;

            if (Near > Far)
            {
                Core::Swap(Near, Far);
            }

            T0 = Max(Near, T0);
            T1 = Min(Far,  T1);
        }

        if (Abs(Delta.GetY()) < k_Epsilon<Base>)
        {
            if (Edge.GetPointA().GetY() < Rectangle.GetTop() || Edge.GetPointA().GetY() > Rectangle.GetBottom())
            {
                return false;
            }
        }
        else
        {
            const Base InvDelta = 1.0 / Delta.GetY();
            Base Near           = (Rectangle.GetTop()    - Edge.GetPointA().GetY()) * InvDelta;
            Base Far            = (Rectangle.GetBottom() - Edge.GetPointA().GetY()) * InvDelta;

            if (Near > Far)
            {
                Core::Swap(Near, Far);
            }

            T0 = Max(Near, T0);
            T1 = Min(Far,  T1);
        }

        if (T0 <= T1)
        {
            if (Manifold)
            {
                const Vector2<Base> Contact = Edge.GetPointA() + Delta * T0;
                Vector2<Base> Normal;
                
                if (T0 > 0.0) 
                {
                    const Base LeftDistance   = Abs(Contact.GetX() - Rectangle.GetLeft());
                    const Base RightDistance  = Abs(Contact.GetX() - Rectangle.GetRight());
                    const Base TopDistance    = Abs(Contact.GetY() - Rectangle.GetTop());
                    const Base BottomDistance = Abs(Contact.GetY() - Rectangle.GetBottom());
                    const Base Distance       = Min(LeftDistance, RightDistance, TopDistance, BottomDistance);

                    if      (Distance == LeftDistance)
                    {
                        Normal = Vector2<Base>(-1, 0);
                    }
                    else if (Distance == RightDistance)
                    {
                        Normal = Vector2<Base>(+1, 0);
                    }
                    else if (Distance == TopDistance)
                    {
                        Normal = Vector2<Base>(0, -1);
                    }
                    else
                    {
                        Normal = Vector2<Base>(0, +1);
                    }
                } 
                else 
                {
                    Normal = Vector2<Base>::Normalize(Rectangle.GetCenter() - Contact);
                }

                Manifold->SetPenetration(Delta.GetLength() * (T1 - T0));
                Manifold->SetNormal(Normal);
                Manifold->AddPoint(Contact);
            }
            return true;
        }
        return false;
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Edge<Base>> Edge, ConstRef<Math::Edge<Base>> Other, Ptr<Math::Manifold<Base>> Manifold)
    {
        return Edge.Intersects(Other, Manifold);
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Edge<Base>> Edge, ConstRef<Math::Circle<Base>> Circle, Ptr<Math::Manifold<Base>> Manifold)
    {
        return Intersects<Base>(Circle, Edge, Manifold);
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Edge<Base>> Edge, ConstRef<Math::Rect<Base>> Rectangle, Ptr<Math::Manifold<Base>> Manifold)
    {
        return Intersects<Base>(Rectangle, Edge, Manifold);
    }

    // -=(Undocumented)=-
    template<typename Base, typename Type, typename Other>
    static constexpr Bool Intersects(ConstPtr<void> First, ConstPtr<void> Second, Ptr<Manifold<Base>> Manifold)
    {
        return Math::Intersects<Base>(* static_cast<ConstPtr<Type>>(First), * static_cast<ConstPtr<Other>>(Second), Manifold);
    }

    // -=(Undocumented)=-
    template<typename Base>
    static constexpr Bool Intersects(ConstRef<Math::Shape<Base>> Shape, ConstRef<Math::Shape<Base>> Other, Ptr<Math::Manifold<Base>> Manifold)
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
            },
        };

        constexpr UInt32 Row    = CastEnum(Shape.GetKind());
        constexpr UInt32 Column = CastEnum(Other.GetKind());
        return k_Dispatch[Row][Column](AddressOf(Shape), AddressOf(Other), Manifold);
    }
}
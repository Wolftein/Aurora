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

#include "Common.hpp"
#include "Aurora.Content/Resource.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Graphic
{
    // -=(Undocumented)=-
    class Mesh final : public Content::AbstractResource<Mesh>
    {
        friend class AbstractResource;

    public:

        // -=(Undocumented)=-
        static constexpr UInt k_MaxBuffers    = 2;

        // -=(Undocumented)=-
        static constexpr UInt k_MaxPrimitives = 16;

        // -=(Undocumented)=-
        struct Attribute
        {
            // -=(Undocumented)=-
            UInt32 Length = 0;

            // -=(Undocumented)=-
            UInt32 Offset = 0;

            // -=(Undocumented)=-
            UInt32 Stride = 0;
        };

        // -=(Undocumented)=-
        struct Primitive
        {
            // -=(Undocumented)=-
            SInt8                             Material = -1;

            // -=(Undocumented)=-
            Attribute                         Indices;

            // -=(Undocumented)=-
            Array<Attribute, k_MaxAttributes> Attributes;

            // -=(Undocumented)=-
            ConstRef<Attribute> GetAttribute(VertexSemantic Semantic) const
            {
                return Attributes[CastEnum(Semantic)];
            }
        };

    public:

        // -=(Undocumented)=-
        explicit Mesh(Any<Content::Uri> Key);

        // -=(Undocumented)=-
        void Load(Any<Data> Vertices, Any<Data> Indices);

        // -=(Undocumented)=-
        Object GetBuffer(Usage Usage) const
        {
            return mBuffers[CastEnum(Usage)];
        }

        // -=(Undocumented)=-
        Bool AddPrimitive(Any<Primitive> Data)
        {
            if (const Ptr<Primitive> Allocation = mPrimitives.Allocate())
            {
                * Allocation = Move(Data);
                return true;
            }
            return false;
        }

        // -=(Undocumented)=-
        ConstRef<Primitive> GetPrimitive(UInt8 Primitive) const
        {
            return mPrimitives[Primitive];
        }

        // -=(Undocumented)=-
        CPtr<const Primitive> GetPrimitives() const
        {
            return mPrimitives.GetData();
        }

    private:

        // \see Resource::OnCreate(Ref<Subsystem::Context>)
        Bool OnCreate(Ref<Subsystem::Context> Context) override;

        // \see Resource::OnDelete(Ref<Subsystem::Context>)
        void OnDelete(Ref<Subsystem::Context> Context) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Array<Data, k_MaxBuffers>         mBytes;
        Array<Object, k_MaxBuffers>       mBuffers;
        Stack<Primitive, k_MaxPrimitives> mPrimitives;
    };
}

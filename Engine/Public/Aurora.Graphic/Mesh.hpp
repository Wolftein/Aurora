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
    /// \brief Represents a mesh asset within the content system.
    class Mesh final : public Content::AbstractResource<Mesh>
    {
        friend class AbstractResource;

    public:

        /// \brief Maximum number of primitives allowed per mesh.
        static constexpr UInt kMaxPrimitives = 16;

        /// \brief Describes a view into a vertex or index buffer for a specific input attribute.
        struct Attribute final
        {
            /// \brief Default-initialized attribute with default values.
            AURORA_INLINE constexpr Attribute() = default;

            /// \brief Constructs an attribute description.
            /// 
            /// \param Length The number of components in the attribute.
            /// \param Offset The byte offset from the start of the vertex to the first component.
            /// \param Stride The byte stride between consecutive vertices.
            AURORA_INLINE constexpr Attribute(UInt32 Length, UInt32 Offset, UInt32 Stride)
                : Length { Length },
                  Offset { Offset },
                  Stride { Stride }
            {
            }

            /// Number of elements in the buffer.
            UInt32 Length = 0;

            /// Byte offset from the start of the buffer to the first element.
            UInt32 Offset = 0;

            /// Byte stride between consecutive elements in the buffer.
            UInt32 Stride = 0;
        };

        /// \brief Represents a drawable subset of a mesh with vertex/index data and optional material.
        struct Primitive final
        {
            /// Index of the material used for this primitive.
            UInt8                            Material = UINT8_MAX;

            /// Index buffer view used to draw this primitive.
            Attribute                        Indices;

            /// Attribute views for vertex input (e.g., position, normal, UV).
            Array<Attribute, kMaxAttributes> Attributes;

            /// \brief Gets the attribute corresponding to the given semantic.
            /// 
            /// \param Semantic The vertex semantic to query.
            /// \return Reference to the corresponding attribute.
            ConstRef<Attribute> GetAttribute(VertexSemantic Semantic) const
            {
                return Attributes[Enum::Cast(Semantic)];
            }
        };

        /// \brief Represents a memory buffer and its associated data blob.
        struct Storage final
        {
            /// CPU-side raw byte data.
            Blob   Data;

            /// GPU-side buffer handle.
            Object Buffer;
        };

    public:

        /// \brief Constructs a mesh resource with the specified content URI.
        /// 
        /// \param Key The URI identifying the mesh asset.
        explicit Mesh(AnyRef<Content::Uri> Key);

        /// \brief Loads the mesh configuration.
        /// 
        /// \param Vertices Binary blob containing vertex buffer data.
        /// \param Indices  Binary blob containing index buffer data.
        void Load(AnyRef<Blob> Vertices, AnyRef<Blob> Indices);

        /// \brief Gets the GPU object representing the vertex buffer.
        /// 
        /// \return Handle to the vertex buffer object.
        AURORA_INLINE Object GetVertices() const
        {
            return mVertices.Buffer;
        }

        /// \brief Gets the GPU object representing the index buffer.
        /// 
        /// \return Handle to the index buffer object.
        AURORA_INLINE Object GetIndices() const
        {
            return mIndices.Buffer;
        }

        /// \brief Adds a new primitive to the mesh.
        /// 
        /// \param Data The primitive to be added to the mesh.
        AURORA_INLINE void AddPrimitive(AnyRef<Primitive> Data)
        {
            mPrimitives.push_back(Move(Data));
        }

        /// \brief Retrieves a specific primitive from the mesh.
        /// 
        /// \param Index Index of the primitive to retrieve.
        /// \return Reference to the requested primitive.
        AURORA_INLINE ConstRef<Primitive> GetPrimitive(UInt8 Index) const
        {
            return mPrimitives[Index];
        }

        /// \brief Returns the list of geometry primitives contained in the mesh.
        /// 
        /// \return Span of all primitives stored in the mesh.
        AURORA_INLINE ConstSpan<Primitive> GetPrimitives() const
        {
            return mPrimitives;
        }

    private:

        /// \copydoc Resource::OnCreate
        Bool OnCreate(Ref<Service::Host> Host) override;

        /// \copydoc Resource::OnDelete
        void OnDelete(Ref<Service::Host> Host) override;

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Storage                           mVertices;
        Storage                           mIndices;
        Vector<Primitive, kMaxPrimitives> mPrimitives;
    };
}

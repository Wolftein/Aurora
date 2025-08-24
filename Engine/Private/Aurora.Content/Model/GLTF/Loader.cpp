// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2025 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Loader.hpp"
#include "Aurora.Content/Service.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Graphic::VertexSemantic As(ConstText Name)
    {
        static TextTable<Graphic::VertexSemantic> kMapping
        {
            { "NORMAL",     Graphic::VertexSemantic::Normal    },
            { "POSITION",   Graphic::VertexSemantic::Position  },
            { "COLOR",      Graphic::VertexSemantic::Color     },
            { "TANGENT",    Graphic::VertexSemantic::Tangent   },
            { "TEXCOORD_0", Graphic::VertexSemantic::TexCoord0 },
            { "TEXCOORD_1", Graphic::VertexSemantic::TexCoord1 },
            { "TEXCOORD_2", Graphic::VertexSemantic::TexCoord2 },
            { "TEXCOORD_3", Graphic::VertexSemantic::TexCoord3 },
        };

        const auto Iterator = kMapping.find(Name.data());
        return (Iterator == kMapping.end() ? Graphic::VertexSemantic::None : Iterator->second);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Graphic::Sampler LoadSampler(ConstRef<tinygltf::Sampler> GLTFSampler)
    {
        Graphic::TextureEdge   EdgeU  = Graphic::TextureEdge::Repeat;
        Graphic::TextureEdge   EdgeV  = Graphic::TextureEdge::Repeat;
        Graphic::TextureFilter Filter = Graphic::TextureFilter::MinMagMipPoint;

        switch (GLTFSampler.wrapS)
        {
        case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
            EdgeU = Graphic::TextureEdge::Clamp;
            break;
        case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
            EdgeU = Graphic::TextureEdge::Mirror;
            break;
        default:
            break;
        }
        switch (GLTFSampler.wrapT)
        {
        case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE:
            EdgeV = Graphic::TextureEdge::Clamp;
            break;
        case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT:
            EdgeV = Graphic::TextureEdge::Mirror;
            break;
        default:
            break;
        }

        switch (GLTFSampler.minFilter)
        {
        case TINYGLTF_TEXTURE_FILTER_NEAREST:
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:
            Filter = Graphic::TextureFilter::MinMagMipPoint;
            break;
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:
            Filter = Graphic::TextureFilter::MinMagMipLinear;
            break;
        case TINYGLTF_TEXTURE_FILTER_LINEAR:
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:
            Filter = Graphic::TextureFilter::MinMagLinearMipPoint;
            break;
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
            Filter = Graphic::TextureFilter::MinMagMipLinear;
            break;
        default:
            break;
        }
        return Graphic::Sampler(EdgeU, EdgeV, Filter);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Tracker<Graphic::Texture> LoadTexture(Ref<tinygltf::Model> GLTFModel, ConstRef<tinygltf::Texture> GLTFTexture)
    {
        ConstRef<tinygltf::Image> GLTFImage = GLTFModel.images[GLTFTexture.source];

        Blob Chunk(GLTFImage.image.size());
        Chunk.Copy(GLTFImage.image.data(), GLTFImage.image.size());

        const Tracker<Graphic::Texture> Texture = Tracker<Graphic::Texture>::Create(Uri(GLTFTexture.name));
        Texture->SetPolicy(Resource::Policy::Exclusive);
        Texture->Load(Graphic::TextureFormat::RGBA8UIntNorm, GLTFImage.width, GLTFImage.height, 1, Move(Chunk));
        return Texture;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool GLTFLoader::Load(Ref<Service> Service, Ref<Scope> Scope, AnyRef<Blob> Data)
    {
        tinygltf::TinyGLTF GLTFLoader;
        tinygltf::Model    GLTFModel;

        // Parse the model asset using TinyGLTF
        Text Error;

        const Bool Result = GLTFLoader.LoadBinaryFromMemory(
            &GLTFModel, &Error, nullptr, Data.GetData<Byte>(), Data.GetSize());
        if (!Result)
        {
            LOG_WARNING("GLTFLoader: {}", Error);
            return false;
        }

        // Safe-guard against big models.
        if (GLTFModel.meshes.size() > Graphic::Mesh::kMaxPrimitives)
        {
            LOG_WARNING("GLTFLoader: Exceeding maximum sub-mesh support of {} with {}",
                Graphic::Mesh::kMaxPrimitives, GLTFModel.meshes.size());
            return false;
        }

        // Find how many bytes we need for buffers and create them
        UInt32 BytesForVertices = 0;
        UInt32 BytesForIndices  = 0;
        for (ConstRef<tinygltf::BufferView> View : GLTFModel.bufferViews)
        {
            if (View.target == TINYGLTF_TARGET_ARRAY_BUFFER)
            {
                BytesForVertices += View.byteLength;
            }
            else
            {
                BytesForIndices += View.byteLength;
            }
        }

        UInt32 OffsetForVertices = 0;
        UInt32 OffsetForIndices  = 0;
        Blob   BlockForVertices(BytesForVertices);
        Blob   BlockForIndices(BytesForIndices);
        for (Ref<tinygltf::BufferView> View : GLTFModel.bufferViews)
        {
            if (View.target == TINYGLTF_TARGET_ARRAY_BUFFER)
            {
                BlockForVertices.Copy(
                    &GLTFModel.buffers[View.buffer].data[View.byteOffset], View.byteLength, OffsetForVertices);
                View.byteOffset = OffsetForVertices;
                OffsetForVertices += View.byteLength;
            }
            else
            {
                BlockForIndices.Copy(
                    &GLTFModel.buffers[View.buffer].data[View.byteOffset], View.byteLength, OffsetForIndices);
                View.byteOffset = OffsetForIndices;
                OffsetForIndices += View.byteLength;
            }
        }

        // Parse each material from the model
        Vector<Tracker<Graphic::Material>, Graphic::Mesh::kMaxPrimitives> Materials { };
        for (ConstRef<tinygltf::Material> GLTFMaterial : GLTFModel.materials)
        {
            const Tracker<Graphic::Material> Material = Tracker<Graphic::Material>::Create(Uri { GLTFMaterial.name });
            Material->SetPolicy(Resource::Policy::Exclusive);

            if (SInt32 Index = GLTFMaterial.pbrMetallicRoughness.baseColorTexture.index; Index >= 0)
            {
                ConstRef<tinygltf::Texture> GLTFTexture = GLTFModel.textures[Index];

                Material->SetTexture(Graphic::TextureSemantic::Diffuse, LoadTexture(GLTFModel, GLTFTexture));
                Material->SetSampler(Graphic::TextureSemantic::Diffuse, LoadSampler(GLTFModel.samplers[GLTFTexture.sampler]));
            }
            if (SInt32 Index = GLTFMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index; Index >= 0)
            {
                ConstRef<tinygltf::Texture> GLTFTexture = GLTFModel.textures[Index];

                Material->SetTexture(Graphic::TextureSemantic::Roughness, LoadTexture(GLTFModel, GLTFTexture));
                Material->SetSampler(Graphic::TextureSemantic::Roughness, LoadSampler(GLTFModel.samplers[GLTFTexture.sampler]));
            }
            if (SInt32 Index = GLTFMaterial.normalTexture.index; Index >= 0)
            {
                ConstRef<tinygltf::Texture> GLTFTexture = GLTFModel.textures[Index];

                Material->SetTexture(Graphic::TextureSemantic::Normal, LoadTexture(GLTFModel, GLTFTexture));
                Material->SetSampler(Graphic::TextureSemantic::Normal, LoadSampler(GLTFModel.samplers[GLTFTexture.sampler]));
            }
            if (SInt32 Index = GLTFMaterial.emissiveTexture.index; Index >= 0)
            {
                ConstRef<tinygltf::Texture> GLTFTexture = GLTFModel.textures[Index];

                Material->SetTexture(Graphic::TextureSemantic::Emissive, LoadTexture(GLTFModel, GLTFTexture));
                Material->SetSampler(Graphic::TextureSemantic::Emissive, LoadSampler(GLTFModel.samplers[GLTFTexture.sampler]));
            }
            if (SInt32 Index = GLTFMaterial.occlusionTexture.index; Index >= 0)
            {
                ConstRef<tinygltf::Texture> GLTFTexture = GLTFModel.textures[Index];

                Material->SetTexture(Graphic::TextureSemantic::Occlusion, LoadTexture(GLTFModel, GLTFTexture));
                Material->SetSampler(Graphic::TextureSemantic::Occlusion, LoadSampler(GLTFModel.samplers[GLTFTexture.sampler]));
            }

            // @TODO: Create uniform buffer for the PBR / Custom properties

            Materials.emplace_back(Material);
        }

        // Parse each mesh from the model
        const Tracker<Graphic::Mesh> Mesh = Tracker<Graphic::Mesh>::Create("Mesh");
        Mesh->SetPolicy(Resource::Policy::Exclusive);
        Mesh->Load(Move(BlockForVertices), Move(BlockForIndices));

        for (ConstRef<tinygltf::Mesh> GLTFMesh : GLTFModel.meshes)
        {
            if (GLTFMesh.primitives.size() > 1)
            {
                LOG_WARNING("GLTFLoader: Multiple primitives unsupported, skipping {}", GLTFMesh.name);
                continue;
            }

            ConstRef<tinygltf::Primitive> GLTFPrimitive = GLTFMesh.primitives[0];

            // Parse material
            Graphic::Mesh::Primitive Primitive;
            Primitive.Material = static_cast<SInt8>(GLTFPrimitive.material);

            // Parse vertices
            for (const auto & [Name, Accessor] : GLTFPrimitive.attributes)
            {
                ConstRef<tinygltf::Accessor>   GLTFAccessor = GLTFModel.accessors[Accessor];
                ConstRef<tinygltf::BufferView> GLTFView     = GLTFModel.bufferViews[GLTFAccessor.bufferView];

                const UInt32 Length = tinygltf::GetComponentSizeInBytes(GLTFAccessor.componentType)
                                    * tinygltf::GetNumComponentsInType(GLTFAccessor.type)
                                    * GLTFAccessor.count;
                const UInt32 Offset = GLTFView.byteOffset + GLTFAccessor.byteOffset;
                const UInt32 Stride = GLTFAccessor.ByteStride(GLTFView);

                if (const Graphic::VertexSemantic Semantic = As(Name); Semantic != Graphic::VertexSemantic::None)
                {
                    Primitive.Attributes[Enum::Cast(Semantic)] = Graphic::Mesh::Attribute(Length, Offset, Stride);
                }
            }

            // Parse indices
            if (GLTFPrimitive.indices >= 0)
            {
                ConstRef<tinygltf::Accessor>   GLTFAccessor = GLTFModel.accessors[GLTFPrimitive.indices];
                ConstRef<tinygltf::BufferView> GLTFView     = GLTFModel.bufferViews[GLTFAccessor.bufferView];

                const UInt32 Length = tinygltf::GetComponentSizeInBytes(GLTFAccessor.componentType)
                                    * tinygltf::GetNumComponentsInType(GLTFAccessor.type)
                                    * GLTFAccessor.count;
                const UInt32 Offset = GLTFView.byteOffset + GLTFAccessor.byteOffset;
                const UInt32 Stride = GLTFAccessor.ByteStride(GLTFView);

                Primitive.Indices = Graphic::Mesh::Attribute(Length, Offset, Stride);
            }

            Mesh->AddPrimitive(Move(Primitive));
        }

        // Load the asset properties
        const Tracker<Graphic::Model> Asset = Tracker<Graphic::Model>::Cast(Scope.GetResource());
        Asset->Load(Mesh, Move(Materials));
        return true;
    }
}
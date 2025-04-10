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
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE_WRITE
#include <tiny_gltf.h>
#include <Aurora.Content/Service.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Graphic::VertexSemantic As(CStr Name)
    {
        static Table<SStr, Graphic::VertexSemantic> k_Mapping
        {
            { "NORMAL",     Graphic::VertexSemantic::Normal },
            { "POSITION",   Graphic::VertexSemantic::Position },
            { "COLOR",      Graphic::VertexSemantic::Color },
            { "TANGENT",    Graphic::VertexSemantic::Tangent },
            { "TEXCOORD_0", Graphic::VertexSemantic::TexCoord0 },
            { "TEXCOORD_1", Graphic::VertexSemantic::TexCoord1 },
            { "TEXCOORD_2", Graphic::VertexSemantic::TexCoord2 },
            { "TEXCOORD_3", Graphic::VertexSemantic::TexCoord3 },
        };

        const auto Iterator = k_Mapping.find(Name.data());
        return (Iterator == k_Mapping.end() ? Graphic::VertexSemantic::None : Iterator->second);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Graphic::Sampler LoadSampler(ConstRef<tinygltf::Sampler> GLTFSampler)
    {
        Graphic::TextureEdge   EdgeU  = Graphic::TextureEdge::Repeat;
        Graphic::TextureEdge   EdgeV  = Graphic::TextureEdge::Repeat;
        Graphic::TextureFilter Filter = Graphic::TextureFilter::Nearest;

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
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST:    // @NOT_SUPPORTED
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR:     // @NOT_SUPPORTED
            Filter = Graphic::TextureFilter::Nearest;
            break;
        case TINYGLTF_TEXTURE_FILTER_LINEAR:
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST:     // @NOT_SUPPORTED
            Filter = Graphic::TextureFilter::Bilinear;
            break;
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR:
             Filter = Graphic::TextureFilter::Trilinear;
            break;
        default:
            break;
        }
        return Graphic::Sampler(EdgeU, EdgeV, Filter);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SPtr<Graphic::Texture> LoadTexture(Ref<tinygltf::Model> GLTFModel, ConstRef<tinygltf::Texture> GLTFTexture)
    {
        ConstRef<tinygltf::Image> GLTFImage = GLTFModel.images[GLTFTexture.source];

        Data Chunk(GLTFImage.image.size());
        Chunk.Copy(GLTFImage.image.data(), GLTFImage.image.size());

        constexpr UInt8 k_DefaultMipmaps = 1;
        constexpr UInt8 k_DefaultSamples = 1;

        const SPtr<Graphic::Texture> Texture = NewPtr<Graphic::Texture>(Uri { GLTFTexture.name });
        Texture->Load(
            Graphic::TextureFormat::RGBA8UIntNorm,
            Graphic::TextureLayout::Source, GLTFImage.width, GLTFImage.height, k_DefaultMipmaps, k_DefaultSamples, Move(Chunk));
        return Texture;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool GLTFLoader::OnLoad(Ref<Service> Service, Any<Data> File, Ref<Graphic::Model> Asset)
    {
        tinygltf::TinyGLTF GLTFLoader;
        tinygltf::Model    GLTFModel;

        // Parse the model asset using TinyGLTF
        SStr Error;

        const Bool Result = GLTFLoader.LoadBinaryFromMemory(
            AddressOf(GLTFModel), AddressOf(Error), nullptr, File.GetData<UInt8>(), File.GetSize());
        if (!Result)
        {
            Log::Warn("GLTFLoader: {}", Error);
            return false;
        }

        // Safe-guard against big model(s).
        if (GLTFModel.meshes.size() > Graphic::Mesh::k_MaxPrimitives)
        {
            Log::Warn("GLTFLoader: Exceeding maximum sub-mesh support of {} with {}",
                Graphic::Mesh::k_MaxPrimitives, GLTFModel.meshes.size());
            return false;
        }

        // Find how many bytes we need for buffer(s) and create them
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
        Data   BlockForVertices(BytesForVertices);
        Data   BlockForIndices(BytesForIndices);
        for (Ref<tinygltf::BufferView> View : GLTFModel.bufferViews)
        {
            if (View.target == TINYGLTF_TARGET_ARRAY_BUFFER)
            {
                memcpy(
                    BlockForVertices.GetData<UInt8>() + OffsetForVertices,
                    AddressOf(GLTFModel.buffers[View.buffer].data[View.byteOffset]), View.byteLength);
                View.byteOffset = OffsetForVertices;
                OffsetForVertices += View.byteLength;
            }
            else
            {
                memcpy(
                     BlockForIndices.GetData<UInt8>() + OffsetForIndices,
                     AddressOf(GLTFModel.buffers[View.buffer].data[View.byteOffset]), View.byteLength);
                View.byteOffset = OffsetForIndices;
                OffsetForIndices += View.byteLength;
            }
        }

        // Parse each material from the model
        Array<SPtr<Graphic::Material>, Graphic::Mesh::k_MaxPrimitives> Materials { };
        for (UInt32 ID = 0; ConstRef<tinygltf::Material> GLTFMaterial : GLTFModel.materials)
        {
            const SPtr<Graphic::Material> Material = NewPtr<Graphic::Material>(Uri { GLTFMaterial.name });
            Material->SetExclusive(true);

            if (SInt32 Index = GLTFMaterial.pbrMetallicRoughness.baseColorTexture.index; Index >= 0)
            {
                ConstRef<tinygltf::Texture> GLTFTexture = GLTFModel.textures[Index];

                Material->SetTexture(Graphic::TextureSlot::Diffuse, LoadTexture(GLTFModel, GLTFTexture));
                Material->SetSampler(Graphic::TextureSlot::Diffuse, LoadSampler(GLTFModel.samplers[GLTFTexture.sampler]));
            }
            if (SInt32 Index = GLTFMaterial.pbrMetallicRoughness.metallicRoughnessTexture.index; Index >= 0)
            {
                ConstRef<tinygltf::Texture> GLTFTexture = GLTFModel.textures[Index];

                Material->SetTexture(Graphic::TextureSlot::Roughness, LoadTexture(GLTFModel, GLTFTexture));
                Material->SetSampler(Graphic::TextureSlot::Roughness, LoadSampler(GLTFModel.samplers[GLTFTexture.sampler]));
            }
            if (SInt32 Index = GLTFMaterial.normalTexture.index; Index >= 0)
            {
                ConstRef<tinygltf::Texture> GLTFTexture = GLTFModel.textures[Index];

                Material->SetTexture(Graphic::TextureSlot::Normal, LoadTexture(GLTFModel, GLTFTexture));
                Material->SetSampler(Graphic::TextureSlot::Normal, LoadSampler(GLTFModel.samplers[GLTFTexture.sampler]));
            }
            if (SInt32 Index = GLTFMaterial.emissiveTexture.index; Index >= 0)
            {
                ConstRef<tinygltf::Texture> GLTFTexture = GLTFModel.textures[Index];

                Material->SetTexture(Graphic::TextureSlot::Emissive, LoadTexture(GLTFModel, GLTFTexture));
                Material->SetSampler(Graphic::TextureSlot::Emissive, LoadSampler(GLTFModel.samplers[GLTFTexture.sampler]));
            }
            if (SInt32 Index = GLTFMaterial.occlusionTexture.index; Index >= 0)
            {
                ConstRef<tinygltf::Texture> GLTFTexture = GLTFModel.textures[Index];

                Material->SetTexture(Graphic::TextureSlot::Occlusion, LoadTexture(GLTFModel, GLTFTexture));
                Material->SetSampler(Graphic::TextureSlot::Occlusion, LoadSampler(GLTFModel.samplers[GLTFTexture.sampler]));
            }

            // @TODO: Create uniform buffer for the PBR / Custom properties

            Materials[ID++] = Material;
        }

        // Parse each mesh from the model
        const SPtr<Graphic::Mesh> Mesh = NewPtr<Graphic::Mesh>(Uri { Asset.GetKey() });
        Mesh->Load(Move(BlockForVertices), Move(BlockForIndices));

        for (ConstRef<tinygltf::Mesh> GLTFMesh : GLTFModel.meshes)
        {
            if (GLTFMesh.primitives.size() > 1)
            {
                Log::Warn("GLTFLoader: Multiple primitives unsupported, skipping {}", GLTFMesh.name);
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
                    Primitive.Attributes[CastEnum(Semantic)] = { Length, Offset, Stride };
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

                Primitive.Indices = { Length, Offset, Stride };
            }

            // Continue with the next primitive
            Mesh->AddPrimitive(Move(Primitive));
        }

        Asset.Load(Mesh, Move(Materials));
        return true;
    }
}
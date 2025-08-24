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

#ifdef    AURORA_PLATFORM_WINDOWS
    #include <d3dcompiler.h>
#endif // AURORA_PLATFORM_WINDOWS

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef    AURORA_PLATFORM_WINDOWS

    static pD3DCompile GetD3DCompileFunction()
    {
        static pD3DCompile D3DCompilePointer = nullptr;

        if (!D3DCompilePointer)
        {
            static constexpr LPCWSTR kFilenames[] = {
                L"D3DCompiler_47",
                L"D3DCompiler_46",
                L"D3DCompiler_43",
                L"D3DCompiler_42",
                L"D3DCompiler_41",
                L"D3DCompiler_40"
            };

            for (const LPCWSTR Library : kFilenames)
            {
                if (const HMODULE Module = ::LoadLibraryW(Library))
                {
                    D3DCompilePointer = (pD3DCompile) GetProcAddress(Module, "D3DCompile");
                    break;
                }
            }
        }
        return D3DCompilePointer;
    }

#endif // AURORA_PLATFORM_WINDOWS

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    PipelineLoader::PipelineLoader(Graphic::Backend Backend, Graphic::Language Target)
        : mBackend { Backend },
          mTarget  { Target }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool PipelineLoader::Load(Ref<Service> Service, Ref<Scope> Scope, AnyRef<Blob> Data)
    {
        Graphic::Descriptor Description;

        TOMLParser        Parser(Data.GetText());
        const TOMLSection Properties = Parser.GetSection("Properties");
        const TOMLSection Program    = Parser.GetSection("Program");

        // Parse 'blend' section
        const TOMLSection Blend = Properties.GetSection("Blend");
        Description.BlendMask           = Enum::Cast(Blend.GetString("Mask"), Graphic::BlendMask::RGBA);
        Description.BlendSrcColor       = Enum::Cast(Blend.GetString("SrcColor"), Graphic::BlendFactor::One);
        Description.BlendDstColor       = Enum::Cast(Blend.GetString("DstColor"), Graphic::BlendFactor::Zero);
        Description.BlendEquationColor  = Enum::Cast(Blend.GetString("EquationColor"), Graphic::BlendFunction::Add);
        Description.BlendSrcAlpha       = Enum::Cast(Blend.GetString("SrcAlpha"), Graphic::BlendFactor::One);
        Description.BlendDstAlpha       = Enum::Cast(Blend.GetString("DstAlpha"), Graphic::BlendFactor::Zero);
        Description.BlendEquationAlpha  = Enum::Cast(Blend.GetString("EquationAlpha"), Graphic::BlendFunction::Add);

        // Parse 'depth' section
        const TOMLSection Depth = Properties.GetSection("Depth");
        Description.DepthClip      = Depth.GetBool("Clip", true);
        Description.DepthMask      = Depth.GetBool("Mask", true);
        Description.DepthTest      = Enum::Cast(Depth.GetString("Condition"), Graphic::TestCondition::LessEqual);
        Description.DepthBias      = Depth.GetReal("Bias", 0.0);
        Description.DepthBiasClamp = Depth.GetReal("BiasClamp", 0.0);
        Description.DepthBiasSlope = Depth.GetReal("BiasSlope", 0.0);

        // Parse 'stencil' section
        const TOMLSection Stencil = Properties.GetSection("Stencil");
        Description.StencilReadMask       = Stencil.GetInteger("ReadMask", 0);
        Description.StencilWriteMask      = Stencil.GetInteger("WriteMask", 0);
        Description.StencilBackTest       = Enum::Cast(Stencil.GetString("BackTest"), Graphic::TestCondition::Always);
        Description.StencilBackFail       = Enum::Cast(Stencil.GetString("BackFail"), Graphic::TestAction::Keep);
        Description.StencilBackDepthFail  = Enum::Cast(Stencil.GetString("BackDepthFail"), Graphic::TestAction::Keep);
        Description.StencilBackDepthPass  = Enum::Cast(Stencil.GetString("BackDepthPass"), Graphic::TestAction::Keep);
        Description.StencilFrontTest      = Enum::Cast(Stencil.GetString("FrontTest"), Graphic::TestCondition::Always);
        Description.StencilFrontFail      = Enum::Cast(Stencil.GetString("FrontFail"), Graphic::TestAction::Keep);
        Description.StencilFrontDepthFail = Enum::Cast(Stencil.GetString("FrontDepthFail"), Graphic::TestAction::Keep);
        Description.StencilFrontDepthPass = Enum::Cast(Stencil.GetString("FrontDepthPass"), Graphic::TestAction::Keep);

        // Parse 'rasterizer' section
        const TOMLSection Rasterizer = Properties.GetSection("Rasterizer");
        Description.Fill = Enum::Cast(Rasterizer.GetString("Fill"), Graphic::Fill::Solid);
        Description.Cull = Enum::Cast(Rasterizer.GetString("Cull"), Graphic::Cull::Back);

        // Parse 'layout' section
        const TOMLSection Layout     = Properties.GetSection("Layout");
        const TOMLArray   Attributes = Layout.GetArray("Attributes");

        for(UInt Index = 0, Size = Attributes.GetSize(); Index < Size; ++Index)
        {
            const TOMLArray Values = Attributes.GetArray(Index);

            Ref<Graphic::Attribute> Attribute = Description.InputAttributes.emplace_back();
            Attribute.Semantic = Enum::Cast(Values.GetString(0), Graphic::VertexSemantic::None);
            Attribute.Format   = Enum::Cast(Values.GetString(1), Graphic::VertexFormat::Float32x4);
            Attribute.Stream   = Values.GetInteger(2);
            Attribute.Offset   = Values.GetInteger(3);
            Attribute.Divisor  = Values.GetInteger(4);
        }

        Description.InputPrimitive = Enum::Cast(Layout.GetString("Primitive"), Graphic::Primitive::TriangleList);

        // Parse 'textures' section
        const TOMLSection Textures = Program.GetSection("Textures");

        Graphic::Pipeline::MappingList<Graphic::TextureSemantic, Graphic::kMaxSlots> TextureMappingList;
        for (const Graphic::TextureSemantic Semantic : Enum::GetValues<Graphic::TextureSemantic>())
        {
            if (const UInt32 Slot = Textures.GetInteger(Enum::GetName(Semantic), ~0); Slot != ~0)
            {
                TextureMappingList.emplace_back(Semantic, Slot);
            }
        }

        // Parse 'shader' section
        Array<Blob, Graphic::kMaxStages> Stages;
        Stages[0] = Compile(Service, Program.GetSection("Vertex"), Graphic::Stage::Vertex);
        Stages[1] = Compile(Service, Program.GetSection("Fragment"), Graphic::Stage::Fragment);

        if (Stages[0] && Stages[1])
        {
            const Tracker<Graphic::Pipeline> Asset = Tracker<Graphic::Pipeline>::Cast(Scope.GetResource());
            Asset->Load(Move(Stages), Move(TextureMappingList), Move(Description));
            return true;
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Blob PipelineLoader::Compile(Ref<Service> Service, ConstRef<TOMLSection> Section, Graphic::Stage Stage)
    {
        if (!Section.IsEmpty())
        {
            const Blob         Data     = Service.Find(Section.GetString("Filename"));
            const ConstText    Code     = Data.GetText();
            const ConstText    Entry    = Section.GetString("Entry", "main");
            const TOMLArray    Defines  = Section.GetArray("Defines");

            if (Code.empty())
            {
                LOG_WARNING("Can't find shader file '{}'", Section.GetString("Filename"));
                return Blob();
            }

            Vector<Property> Properties;
            Properties.reserve(Defines.GetSize());

            for (UInt32 Index = 0, Limit = Defines.GetSize(); Index < Limit; ++Index)
            {
                const ConstText Definition = Defines.GetString(Index);
                const UInt Delimiter       = Definition.find_first_of('=');

                Properties.emplace_back(
                    Delimiter != ConstText::npos ? Definition.substr(0, Delimiter) : Definition,
                    Delimiter != ConstText::npos ? Definition.substr(Delimiter) : "true");
            }

#ifdef    AURORA_PLATFORM_WINDOWS
            return CompileDXBC(Entry, Code, Properties, Stage);
#endif // AURORA_PLATFORM_WINDOWS
        }
        return Blob();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Blob PipelineLoader::CompileDXBC(ConstText Entry, ConstText Code, Ref<Vector<Property>> Properties, Graphic::Stage Stage)
    {
        Blob Compilation;

#ifdef    AURORA_PLATFORM_WINDOWS
        D3D_SHADER_MACRO Defines[128];

        for (UInt Index = 0; Index < Properties.size(); ++Index)
        {
            Defines[Index].Name       = Properties[Index].Key.c_str();
            Defines[Index].Definition = Properties[Index].Value.c_str();
        }
        Defines[Properties.size()] = { nullptr, nullptr };

        Ptr<ID3DBlob> Error    = nullptr;
        Ptr<ID3DBlob> Bytecode = nullptr;

        constexpr ConstText kShaderProfiles[][3] = {
            { "vs_4_0_level_9_1", "ps_4_0_level_9_1", ""       },
            { "vs_4_0_level_9_1", "ps_4_0_level_9_1", ""       },
            { "vs_4_0_level_9_3", "ps_4_0_level_9_3", ""       },
            { "vs_4_0",           "ps_4_0",           "gs_4_0" },
            { "vs_5_0",           "ps_5_0",           "gs_5_0" },
            { "vs_6_0",           "ps_6_0",           "gs_6_0" }
        };

        ConstRef<ConstText> Profile = kShaderProfiles[Enum::Cast(mTarget)][Enum::Cast(Stage)];

        if (const pD3DCompile D3DCompile = GetD3DCompileFunction())
        {
            const HRESULT Result   = D3DCompile(
                Code.data(),
                Code.size(),
                nullptr,
                Defines,
                nullptr,
                Entry.data(),
                Profile.data(),
                D3DCOMPILE_OPTIMIZATION_LEVEL3,
                0,
                &Bytecode,
                &Error);

            if (SUCCEEDED(Result))
            {
                Compilation = Blob(Bytecode->GetBufferSize());
                Compilation.Copy(Bytecode->GetBufferPointer(), Bytecode->GetBufferSize());
                Bytecode->Release();
            }
            else
            {
                LOG_ERROR("Failed to compile shader: {}", reinterpret_cast<Ptr<Char>>(Error->GetBufferPointer()));
            }
        }
        else
        {
            LOG_ERROR("Can't find D3DCompile function");
        }
#endif // AURORA_PLATFORM_WINDOWS

        return Compilation;
    }
}
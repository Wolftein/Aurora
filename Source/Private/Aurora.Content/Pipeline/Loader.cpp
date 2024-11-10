// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
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
#include "Aurora.Graphic/Shader.hpp"

#ifdef    SDL_PLATFORM_WINDOWS
    #include <d3dcompiler.h>
#endif // SDL_PLATFORM_WINDOWS

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifdef    SDL_PLATFORM_WINDOWS

    static pD3DCompile GetD3DCompileFunction()
    {
        static pD3DCompile FunctionPtr = nullptr;

        if (FunctionPtr == nullptr)
        {
            static constexpr LPCWSTR kLibraries[] = {
                L"D3DCompiler_47",
                L"D3DCompiler_46",
                L"D3DCompiler_43",
                L"D3DCompiler_42",
                L"D3DCompiler_41",
                L"D3DCompiler_40"
            };

            for (LPCWSTR Library : kLibraries)
            {
                if (const HMODULE Module = ::LoadLibraryW(Library))
                {
                    FunctionPtr = (pD3DCompile) GetProcAddress(Module, "D3DCompile");
                    break;
                }
            }
        }
        return FunctionPtr;
    }

#endif // SDL_PLATFORM_WINDOWS

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    PipelineLoader::PipelineLoader(Graphic::Backend Backend, Graphic::Language Target)
        : mBackend { Backend },
          mTarget  { Target }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool PipelineLoader::OnLoad(Ref<Service> Service, Any<Data> File, Ref<Graphic::Pipeline> Asset)
    {
        Graphic::Descriptor Description;

        TOMLParser        Parser(File.GetText());
        const TOMLSection Properties = Parser.GetSection("Properties");
        const TOMLSection Program    = Parser.GetSection("Program");

        // Parse 'blend' section
        const TOMLSection Blend = Properties.GetSection("Blend");
        Description.BlendMask           = CastEnum(Blend.GetString("Mask"), Graphic::BlendColor::RGBA);
        Description.BlendColorSrcFactor = CastEnum(Blend.GetString("ColorSrcFactor"), Graphic::BlendFactor::One);
        Description.BlendColorDstFactor = CastEnum(Blend.GetString("ColorDstFactor"), Graphic::BlendFactor::Zero);
        Description.BlendColorEquation  = CastEnum(Blend.GetString("ColorEquation"), Graphic::BlendFunction::Add);
        Description.BlendAlphaSrcFactor = CastEnum(Blend.GetString("AlphaSrcFactor"), Graphic::BlendFactor::One);
        Description.BlendAlphaDstFactor = CastEnum(Blend.GetString("AlphaDstFactor"), Graphic::BlendFactor::Zero);
        Description.BlendAlphaEquation  = CastEnum(Blend.GetString("AlphaEquation"), Graphic::BlendFunction::Add);

        // Parse 'depth' section
        const TOMLSection Depth = Properties.GetSection("Depth");
        Description.DepthMask      = Depth.GetBool("Mask", true);
        Description.DepthCondition = CastEnum(Depth.GetString("Condition"), Graphic::TestCondition::LessEqual);

        // Parse 'stencil' section
        const TOMLSection Stencil = Properties.GetSection("Stencil");
        Description.StencilMask        = Stencil.GetBool("Mask", false);
        Description.StencilCondition   = CastEnum(Stencil.GetString("Condition"), Graphic::TestCondition::Always);
        Description.StencilOnFail      = CastEnum(Stencil.GetString("OnFail"), Graphic::TestAction::Keep);
        Description.StencilOnDepthFail = CastEnum(Stencil.GetString("OnDepthFail"), Graphic::TestAction::Keep);
        Description.StencilOnDepthPass = CastEnum(Stencil.GetString("OnDepthPass"), Graphic::TestAction::Keep);

        // Parse 'rasterizer' section
        const TOMLSection Rasterizer = Properties.GetSection("Rasterizer");
        Description.Fill = Rasterizer.GetBool("Fill", true);
        Description.Cull = CastEnum(Rasterizer.GetString("Cull"), Graphic::Cull::Back);

        // Parse 'layout' section
        const TOMLSection Layout     = Properties.GetSection("Layout");
        const TOMLArray   Attributes = Layout.GetArray("Attributes");

        if (Attributes.IsNull() || Attributes.IsEmpty())
        {
            Description.InputLayout[0] = { Graphic::VertexSemantic::Position,  Graphic::VertexFormat::Float32x3,   0, 0,  };
            Description.InputLayout[1] = { Graphic::VertexSemantic::Color,     Graphic::VertexFormat::UIntNorm8x4, 0, 12, };
            Description.InputLayout[2] = { Graphic::VertexSemantic::TexCoord0, Graphic::VertexFormat::Float32x2,   0, 16, };
        }
        else
        {
            for(UInt Attribute = 0, Size = Attributes.GetSize(); Attribute < Size; ++Attribute)
            {
                const TOMLArray Values = Attributes.GetArray(Attribute);

                Description.InputLayout[Attribute].ID     = CastEnum(Values.GetString(0), Graphic::VertexSemantic::None);
                Description.InputLayout[Attribute].Format = CastEnum(Values.GetString(1), Graphic::VertexFormat::Float32x4);
                Description.InputLayout[Attribute].Slot   = Values.GetNumber(2);
                Description.InputLayout[Attribute].Offset = Values.GetNumber(3);
            }
        }

        Description.InputTopology = CastEnum(Layout.GetString("Topology"), Graphic::VertexTopology::Triangle);

        // Parse 'textures' section
        const TOMLSection Textures = Program.GetSection("Textures");

        Array<Graphic::Source, Graphic::k_MaxSlots> Slots;
        for (const Graphic::Source Source : magic_enum::enum_values<Graphic::Source>())
        {
            if (const SInt32 Slot = Textures.GetNumber(magic_enum::enum_name(Source), -1); Slot != -1)
            {
                Slots[Slot] = Source;
            }
        }

        // Parse 'shader' section
        Array<Data, Graphic::k_MaxStages> Stages;
        Stages[0] = Compile(Service, Program.GetSection("Vertex"), Graphic::Stage::Vertex);
        Stages[1] = Compile(Service, Program.GetSection("Fragment"), Graphic::Stage::Fragment);
        Stages[2] = Compile(Service, Program.GetSection("Geometry"), Graphic::Stage::Geometry);

        if (Stages[0].HasData() && Stages[1].HasData())
        {
            Asset.Load(Move(Stages), Move(Slots), Move(Description));
            return true;
        }

        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Data PipelineLoader::Compile(Ref<Service> Service, Ref<const TOMLSection> Section, Graphic::Stage Stage)
    {
        if (!Section.IsEmpty())
        {
            ConstSPtr<Graphic::Shader> Shader = Service.Load<Graphic::Shader>(Section.GetString("Filename"));
            const CStr         Code     = Shader->GetBytecode();
            const CStr         Entry    = Section.GetString("Entry", "main");
            const Vector<CStr> Defines  = Section.GetStringArray("Defines");

            Vector<Property> Properties(Defines.size());

            for (CStr Definition : Defines)
            {
                const UInt Delimiter = Definition.find_first_of('=');

                const SStr Name(Delimiter != CStr::npos ? Definition.substr(0, Delimiter) : Definition);
                const SStr Data(Delimiter != CStr::npos ? Definition.substr(Delimiter) : "true");

                Properties.emplace_back(Property { Name, Data });
            }

#ifdef    SDL_PLATFORM_WINDOWS
            return CompileDXBC(Entry, Code, Properties, Stage);
#endif // SDL_PLATFORM_WINDOWS
        }
        return Data();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Data PipelineLoader::CompileDXBC(CStr Entry, CStr Code, Ref<Vector<Property>> Properties, Graphic::Stage Stage)
    {
        Data Compilation;

#ifdef    SDL_PLATFORM_WINDOWS
        D3D_SHADER_MACRO Defines[256];

        for (UInt Index = 0; Index < Properties.size(); ++Index)
        {
            Defines[Index].Name       = Properties[Index].Name.c_str();
            Defines[Index].Definition = Properties[Index].Definition.c_str();
        }
        Defines[Properties.size()] = { nullptr, nullptr };

        Ptr<ID3DBlob> Error    = nullptr;
        Ptr<ID3DBlob> Bytecode = nullptr;

        constexpr CStr kShaderProfiles[][3] = {
            { "vs_4_0_level_9_1", "ps_4_0_level_9_1", ""       },
            { "vs_4_0_level_9_1", "ps_4_0_level_9_1", ""       },
            { "vs_4_0_level_9_3", "ps_4_0_level_9_3", ""       },
            { "vs_4_0",           "ps_4_0",           "gs_4_0" },
            { "vs_5_0",           "ps_5_0",           "gs_5_0" },
            { "vs_6_0",           "ps_6_0",           "gs_6_0" }
        };

        Ref<const CStr> Profile = kShaderProfiles[CastEnum(mTarget)][CastEnum(Stage)];

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
                & Bytecode,
                & Error);

            if (SUCCEEDED(Result))
            {
                const auto OnRelease = [Bytecode](Ptr<void> Address) {
                    Bytecode->Release();
                };
                Compilation = Data(Bytecode->GetBufferPointer(), Bytecode->GetBufferSize(), OnRelease);
            }
            else
            {
                Log::Error("Failed to compile shader: {}", reinterpret_cast<Ptr<Char>>(Error->GetBufferPointer()));
            }
        }
        else
        {
            Log::Error("Can't find D3DCompile function");
        }
#endif // SDL_PLATFORM_WINDOWS

        return Compilation;
    }
}
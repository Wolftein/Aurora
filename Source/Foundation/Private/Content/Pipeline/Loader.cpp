// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "Loader.hpp"
#include <Graphic/Shader.hpp>
#include <Content/Service.hpp>

#ifdef    EA_PLATFORM_WINDOWS
    #include <d3dcompiler.h>
#endif // EA_PLATFORM_WINDOWS

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto ParseBlendColor(CStr Key)
    {
        static Table<CStr, Graphic::BlendColor> k_Mapping {
            { "RGBA", Graphic::BlendColor::RGBA },
            { "RBA",  Graphic::BlendColor::RBA  },
            { "RGA",  Graphic::BlendColor::RGA  },
            { "RGB",  Graphic::BlendColor::RGB  },
            { "GBA",  Graphic::BlendColor::GBA  },
            { "RA",   Graphic::BlendColor::RA   },
            { "RB",   Graphic::BlendColor::RB   },
            { "RG",   Graphic::BlendColor::RG   },
            { "GA",   Graphic::BlendColor::GA   },
            { "GB",   Graphic::BlendColor::GB   },
            { "BA",   Graphic::BlendColor::BA   },
            { "R",    Graphic::BlendColor::R    },
            { "G",    Graphic::BlendColor::G    },
            { "B",    Graphic::BlendColor::B    },
            { "A",    Graphic::BlendColor::A    },
            { "NONE", Graphic::BlendColor::None },
        };
        return k_Mapping[Key];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto ParseBlendFactor(CStr Key)
    {
        static Table<CStr, Graphic::BlendFactor> k_Mapping {
            { "One",              Graphic::BlendFactor::One                },
            { "Zero",             Graphic::BlendFactor::Zero               },
            { "DstAlpha",         Graphic::BlendFactor::DstAlpha           },
            { "DstColor",         Graphic::BlendFactor::DstColor           },
            { "SrcAlpha",         Graphic::BlendFactor::SrcAlpha           },
            { "SrcColor",         Graphic::BlendFactor::SrcColor           },
            { "OneMinusDstAlpha", Graphic::BlendFactor::OneMinusDstAlpha   },
            { "OneMinusDstColor", Graphic::BlendFactor::OneMinusDstColor   },
            { "OneMinusSrcAlpha", Graphic::BlendFactor::OneMinusSrcAlpha   },
            { "OneMinusSrcColor", Graphic::BlendFactor::OneMinusSrcColor   },
        };
        return k_Mapping[Key];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto ParseBlendFunction(CStr Key)
    {
        static Table<CStr, Graphic::BlendFunction> k_Mapping {
            { "Add",             Graphic::BlendFunction::Add             },
            { "Subtract",        Graphic::BlendFunction::Subtract        },
            { "ReverseSubtract", Graphic::BlendFunction::ReverseSubtract },
            { "Min",             Graphic::BlendFunction::Min             },
            { "Max",             Graphic::BlendFunction::Max             },
        };
        return k_Mapping[Key];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto ParseCull(CStr Key)
    {
        static Table<CStr, Graphic::Cull> k_Mapping {
            { "None",  Graphic::Cull::None  },
            { "Back",  Graphic::Cull::Back  },
            { "Front", Graphic::Cull::Front },
        };
        return k_Mapping[Key];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto ParseTestAction(CStr Key)
    {
        static Table<CStr, Graphic::TestAction> k_Mapping {
            { "Zero",          Graphic::TestAction::Zero          },
            { "Keep",          Graphic::TestAction::Keep          },
            { "Invert",        Graphic::TestAction::Invert        },
            { "Replace",       Graphic::TestAction::Replace       },
            { "Decrement",     Graphic::TestAction::Decrement     },
            { "Increment",     Graphic::TestAction::Increment     },
            { "DecrementWrap", Graphic::TestAction::DecrementWrap },
            { "IncrementWrap", Graphic::TestAction::IncrementWrap },
        };
        return k_Mapping[Key];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto ParseTestCondition(CStr Key)
    {
        static Table<CStr, Graphic::TestCondition> k_Mapping {
            { "Less",         Graphic::TestCondition::Less         },
            { "Equal",        Graphic::TestCondition::Equal        },
            { "Never",        Graphic::TestCondition::Never        },
            { "Always",       Graphic::TestCondition::Always       },
            { "Greater",      Graphic::TestCondition::Greater      },
            { "NotEqual",     Graphic::TestCondition::NotEqual     },
            { "LessEqual",    Graphic::TestCondition::LessEqual    },
            { "GreaterEqual", Graphic::TestCondition::GreaterEqual },
        };
        return k_Mapping[Key];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto ParseVertexFormat(CStr Key)
    {
        static Table<CStr, Graphic::VertexFormat> k_Mapping {
            { "Float16x2",      Graphic::VertexFormat::Float16x2        },
            { "Float16x4",      Graphic::VertexFormat::Float16x4        },
            { "Float32x1",      Graphic::VertexFormat::Float32x1        },
            { "Float32x2",      Graphic::VertexFormat::Float32x2        },
            { "Float32x3",      Graphic::VertexFormat::Float32x3        },
            { "Float32x4",      Graphic::VertexFormat::Float32x4        },
            { "SInt8x4",        Graphic::VertexFormat::SInt8x4          },
            { "SIntNorm8x4",    Graphic::VertexFormat::SIntNorm8x4      },
            { "UInt8x4",        Graphic::VertexFormat::UInt8x4          },
            { "UIntNorm8x4",    Graphic::VertexFormat::UIntNorm8x4      },
            { "SInt16x2",       Graphic::VertexFormat::SInt16x2         },
            { "SIntNorm16x2",   Graphic::VertexFormat::SIntNorm16x2     },
            { "UInt16x2",       Graphic::VertexFormat::UInt16x2         },
            { "UIntNorm16x2",   Graphic::VertexFormat::UIntNorm16x2     },
            { "SInt16x4",       Graphic::VertexFormat::SInt16x4         },
            { "SIntNorm16x4",   Graphic::VertexFormat::SIntNorm16x4     },
            { "UInt16x4",       Graphic::VertexFormat::UInt16x4         },
            { "UIntNorm16x4",   Graphic::VertexFormat::UIntNorm16x4     },
        };
        return k_Mapping[Key];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto ParseVertexTopology(CStr Key)
    {
        static Table<CStr, Graphic::VertexTopology> k_Mapping {
            { "Point",      Graphic::VertexTopology::Point       },
            { "Line",       Graphic::VertexTopology::Line        },
            { "Triangle",   Graphic::VertexTopology::Triangle    },
        };
        return k_Mapping[Key];
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    PipelineLoader::PipelineLoader(Graphic::Backend Backend, Graphic::Language Target)
        : mBackend { Backend },
          mTarget  { Target }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool PipelineLoader::Load(ConstSPtr<Service> Service, Ref<Chunk> Data, ConstSPtr<Graphic::Pipeline> Asset)
    {
        Graphic::Descriptor Description;

        TOMLParser        Parser(Data.GetText());
        const TOMLSection Properties = Parser.GetSection("Properties");

        // Parse 'blend' section
        const TOMLSection Blend = Properties.GetSection("Blend");
        Description.BlendMask           = ParseBlendColor(Blend.GetString("Mask", "RGBA"));
        Description.BlendColorSrcFactor = ParseBlendFactor(Blend.GetString("ColorSrcFactor", "One"));
        Description.BlendColorDstFactor = ParseBlendFactor(Blend.GetString("ColorDstFactor", "Zero"));
        Description.BlendColorEquation  = ParseBlendFunction(Blend.GetString("ColorEquation", "Add"));
        Description.BlendAlphaSrcFactor = ParseBlendFactor(Blend.GetString("AlphaSrcFactor", "One"));
        Description.BlendAlphaDstFactor = ParseBlendFactor(Blend.GetString("AlphaDstFactor", "Zero"));
        Description.BlendAlphaEquation  = ParseBlendFunction(Blend.GetString("AlphaEquation", "Add"));

        // Parse 'depth' section
        const TOMLSection Depth = Properties.GetSection("Depth");
        Description.DepthMask      = Depth.GetBool("Mask", true);
        Description.DepthCondition = ParseTestCondition(Depth.GetString("Condition", "LessEqual"));

        // Parse 'stencil' section
        const TOMLSection Stencil = Properties.GetSection("Stencil");
        Description.StencilMask        = Stencil.GetBool("Mask", false);
        Description.StencilCondition   = ParseTestCondition(Stencil.GetString("Condition", "Always"));
        Description.StencilOnFail      = ParseTestAction(Stencil.GetString("OnFail", "Keep"));
        Description.StencilOnDepthFail = ParseTestAction(Stencil.GetString("OnDepthFail", "Keep"));
        Description.StencilOnDepthPass = ParseTestAction(Stencil.GetString("OnDepthPass", "Keep"));

        // Parse 'rasterizer' section
        const TOMLSection Rasterizer = Properties.GetSection("Rasterizer");
        Description.Fill = Rasterizer.GetBool("Fill", true);
        Description.Cull = ParseCull(Rasterizer.GetString("Cull", "Back"));

        // Parse 'layout' section
        const TOMLSection Layout     = Properties.GetSection("Layout");
        const TOMLArray   Attributes = Layout.GetArray("Attributes");

        if (Attributes.IsNull() || Attributes.IsEmpty())
        {
            Description.InputLayout[0] = { 0, 0,  Graphic::VertexFormat::Float32x3   };
            Description.InputLayout[1] = { 1, 12, Graphic::VertexFormat::UIntNorm8x4 };
            Description.InputLayout[2] = { 2, 16, Graphic::VertexFormat::Float32x2   };
        }
        else
        {
            for(UInt Attribute = 0, Size = Attributes.GetSize(); Attribute < Size; ++Attribute)
            {
                const TOMLArray Values = Attributes.GetArray(Attribute);

                Description.InputLayout[Attribute].Slot   = Values.GetNumber(0);
                Description.InputLayout[Attribute].Offset = Values.GetNumber(1);
                Description.InputLayout[Attribute].Format = ParseVertexFormat(Values.GetString(2));
            }
        }

        Description.InputTopology = ParseVertexTopology(Layout.GetString("Topology", "Triangle"));

        // Parse program section by loading each shader
        const TOMLSection Program = Parser.GetSection("Program");

        Array<Chunk, Graphic::k_MaxStages> Bytecodes;
        Bytecodes[0] = Compile(Service, Program.GetSection("Vertex"), Graphic::Stage::Vertex);
        Bytecodes[1] = Compile(Service, Program.GetSection("Fragment"), Graphic::Stage::Fragment);
        Bytecodes[2] = Compile(Service, Program.GetSection("Geometry"), Graphic::Stage::Geometry);

        if (Bytecodes[0].HasData() && Bytecodes[1].HasData())
        {
            Asset->Load(Bytecodes, Description);
            return true;
        }

        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Chunk PipelineLoader::Compile(ConstSPtr<Service> Service, Ref<const TOMLSection> Section, Graphic::Stage Stage)
    {
        if (Section.IsEmpty())
        {
            return Chunk();
        }
        else
        {
            ConstSPtr<Graphic::Shader> Shader = Service->Load<Graphic::Shader>(Section.GetString("Filename"));
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

#ifdef    EA_PLATFORM_WINDOWS
            return CompileDXBC(Entry, Code, Properties, Stage);
#endif // EA_PLATFORM_WINDOWS
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Chunk PipelineLoader::CompileDXBC(CStr Entry, CStr Code, Ref<Vector<Property>> Properties, Graphic::Stage Stage)
    {
#ifdef    EA_PLATFORM_WINDOWS
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

        Chunk Compilation;

        if (SUCCEEDED(Result))
        {
            Compilation = Chunk(Bytecode->GetBufferSize());
            FastCopyMemory(Compilation.GetData(), Bytecode->GetBufferPointer(), Bytecode->GetBufferSize());
            Bytecode->Release();
        }
        else
        {
            LOG_ERROR("Failed to compile shader: {}", reinterpret_cast<Ptr<Char>>(Error->GetBufferPointer()));
        }

        return Compilation;
#else  // EA_PLATFORM_WINDOWS
        return Chunk();
#endif // EA_PLATFORM_WINDOWS
    }
}
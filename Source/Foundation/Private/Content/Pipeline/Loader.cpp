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

    Bool PipelineLoader::Load(Ref<Chunk> Data, Ref<const SPtr<Graphic::Pipeline>> Asset)
    {
        Graphic::Descriptor Description;

        TOMLParser        Parser(Data.GetText());
        const TOMLSection Properties = Parser.GetSection("Properties");
        const TOMLSection Program    = Parser.GetSection("Program");

        // Parse 'blend' section
        const TOMLSection Blend = Properties.GetSection("Blend");
        Description.BlendMask           = ParseBlendColor(Blend.GetString("Mask", "RGBA"));
        Description.BlendColorSrcFactor = ParseBlendFactor(Blend.GetString("ColorSrcFactor", "One"));
        Description.BlendColorDstFactor = ParseBlendFactor(Blend.GetString("ColorDstFactor", "Zero"));
        Description.BlendColorEquation  = ParseBlendFunction(Blend.GetString("ColorEquation", "Add"));
        Description.BlendAlphaSrcFactor = ParseBlendFactor(Blend.GetString("AlphaSrcFactor", "One"));
        Description.BlendAlphaDstFactor = ParseBlendFactor(Blend.GetString("AlphaSrcFactor", "Zero"));
        Description.BlendAlphaEquation  = ParseBlendFunction(Blend.GetString("AlphaEquation", "Add"));

        // Parse 'depth' section
        const TOMLSection Depth = Properties.GetSection("Depth");
        Description.DepthMask      = Depth.GetBool("Mask", true);
        Description.DepthCondition = ParseTestCondition(Depth.GetString("Condition", "LessEqual"));

        // Parse 'stencil' section
        const TOMLSection Stencil = Properties.GetSection("Stencil");
        Description.StencilMask      = Stencil.GetBool("Mask", false);
        Description.StencilCondition = ParseTestCondition(Stencil.GetString("Condition", "LessEqual"));
        Description.StencilOnFail    = ParseTestAction(Stencil.GetString("OnFail", "Always"));
        Description.StencilOnZFail   = ParseTestAction(Stencil.GetString("OnZFail", "Always"));
        Description.StencilOnZPass   = ParseTestAction(Stencil.GetString("OnZPass", "Always"));

        // Parse 'rasterizer' section
        const TOMLSection Rasterizer = Properties.GetSection("Rasterizer");
        Description.Fill = Rasterizer.GetBool("Fill", false);
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

        // TODO: Load code from separate file with defines and includes, etc.
        Chunk Chunk_1;
        Chunk Chunk_2;

        switch (mBackend)
        {
        case Graphic::Backend::Direct3D11:
            Chunk_1 = CompileDXBC(Program.GetString("Code"), Graphic::Stage::Vertex);
            Chunk_2 = CompileDXBC(Program.GetString("Code"), Graphic::Stage::Fragment);
            break;
        default:
            break;
        }

        if (Chunk_1.HasData() && Chunk_2.HasData())
        {
            Asset->Load(Chunk_1, Chunk_2, Description);
            return true;
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Chunk PipelineLoader::CompileDXBC(CStr Code, Graphic::Stage Stage)
    {
#ifdef    EA_PLATFORM_WINDOWS
        Ptr<ID3DBlob> Error    = nullptr;
        Ptr<ID3DBlob> Bytecode = nullptr;

        Ptr<const char> Target = "";
        Ptr<const char> Entry  = (Stage == Graphic::Stage::Vertex ? "vertex" : "pixel");

        switch (mTarget)
        {
        case Graphic::Language::Version_1:
        case Graphic::Language::Version_2:
            Target = (Stage == Graphic::Stage::Vertex ? "vs_4_0_level_9_1" : "ps_4_0_level_9_1");
            break;
        case Graphic::Language::Version_3:
            Target = (Stage == Graphic::Stage::Vertex ? "vs_4_0_level_9_3" : "ps_4_0_level_9_3");
            break;
        case Graphic::Language::Version_4:
            Target = (Stage == Graphic::Stage::Vertex ? "vs_4_0" : "ps_4_0");
            break;
        case Graphic::Language::Version_5:
            Target = (Stage == Graphic::Stage::Vertex ? "vs_5_0" : "ps_5_0");
            break;
        case Graphic::Language::Version_6:
            Target = (Stage == Graphic::Stage::Vertex ? "vs_6_0" : "ps_6_0");
            break;
        }

        const HRESULT Result   = D3DCompile(
            Code.data(),
            Code.size(),
            nullptr,
            nullptr,
            nullptr,
            Entry,
            Target,
            D3DCOMPILE_OPTIMIZATION_LEVEL3,
            0,
            & Bytecode,
            & Error);

        if (FAILED(Result))
        {
            OutputDebugStringA(reinterpret_cast<Ptr<Char>>(Error->GetBufferPointer())); // TODO: Replace with Logging
            return Chunk();
        }

        // TODO: Zero Copy memory
        Chunk Chunk(Bytecode->GetBufferSize());
        memcpy(Chunk.GetData(), Bytecode->GetBufferPointer(), Bytecode->GetBufferSize());
        Bytecode->Release();

        return Chunk;
#else  // EA_PLATFORM_WINDOWS
        return Chunk();
#endif // EA_PLATFORM_WINDOWS
    }
}
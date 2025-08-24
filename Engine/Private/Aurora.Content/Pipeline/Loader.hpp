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

#include "Aurora.Content/Loader.hpp"
#include "Aurora.Graphic/Pipeline.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    /// \brief Represents the \ref Loader for pipeline “effect” files (pipeline, shaders, and metadata).
    class PipelineLoader final : public Loader
    {
    public:

        /// \brief Creates a loader with a concrete GPU backend and target shading language.
        /// 
        /// \param Backend Graphics backend to generate artifacts for (e.g., D3D, GL, VK).
        /// \param Target  Shading language to emit for the backend (e.g., HLSL, GLSL).
        PipelineLoader(Graphic::Backend Backend, Graphic::Language Target);

        /// \copydoc Loader::GetExtensions
        ConstSpan<ConstText> GetExtensions() const override
        {
            static constexpr List<ConstText> EXTENSION_LIST = { "effect" };
            return EXTENSION_LIST;
        }

        /// \copydoc Loader::Load
        Bool Load(Ref<Service> Service, Ref<Scope> Scope, AnyRef<Blob> Data) override;

    private:

        /// \brief Key–value property used during compilation (e.g., macros or options).
        struct Property final
        {
            /// \brief Default constructor.
            AURORA_INLINE constexpr Property() = default;

            /// \brief Builds a property with a key and value.
            /// 
            /// \param Key   Property name (e.g., define or option).
            /// \param Value Property value as text.
            AURORA_INLINE constexpr Property(ConstText Key, ConstText Value)
                : Key   { Key },
                  Value { Value }
            {
            }

            /// Property name.
            Text Key;

            /// Property value.
            Text Value;
        };

    private:

        /// \brief Compiles a single stage described in a TOML effect section.
        /// 
        /// \param Service Asset service for includes and tool access.
        /// \param Section Parsed TOML section that describes this stage.
        /// \param Stage   Shader stage to compile (vertex, fragment, etc.).
        /// \return Compiled bytecode or IR for the chosen backend and language.
        Blob Compile(Ref<Service> Service, ConstRef<TOMLSection> Section, Graphic::Stage Stage);

        /// \brief Compiles HLSL source to DXBC/DXIL bytecode with optional properties.
        /// 
        /// \param Entry      HLSL entry-point name.
        /// \param Code       HLSL source text.
        /// \param Properties Optional properties (e.g., defines) applied at compile time.
        /// \param Stage      Shader stage to compile.
        /// \return Direct3D bytecode blob suitable for pipeline creation.
        Blob CompileDXBC(ConstText Entry, ConstText Code, Ref<Vector<Property>> Properties, Graphic::Stage Stage);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Graphic::Backend  mBackend;
        Graphic::Language mTarget;
    };
}
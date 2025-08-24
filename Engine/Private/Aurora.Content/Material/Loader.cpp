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

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool MaterialLoader::Load(Ref<Service> Service, Ref<Scope> Scope, AnyRef<Blob> Data)
    {
        TOMLParser        Parser(Data.GetText());
        const TOMLSection Root     = Parser.GetSection("Material");
        const TOMLSection Textures = Parser.GetSection("Textures");
        const TOMLSection Samplers = Parser.GetSection("Samplers");

        // Parse 'properties' section
        const Tracker<Graphic::Material> Asset = Tracker<Graphic::Material>::Cast(Scope.GetResource());
        Asset->SetKind(Enum::Cast(Root.GetString("Kind"), Graphic::Material::Kind::Opaque));

        // Parse 'textures' section
        for (const Graphic::TextureSemantic Semantic : Enum::GetValues<Graphic::TextureSemantic>())
        {
            if (const TOMLSection Texture = Textures.GetSection(Enum::GetName(Semantic), false); !Texture.IsNull())
            {
                // Parse texture
                Asset->SetTexture(Semantic, Service.Load<Graphic::Texture>(Texture.GetString("Path"), &Scope));

                // Parse sampler
                if (const TOMLSection Sampler = Samplers.GetSection(Texture.GetString("Sampler")); !Sampler.IsNull())
                {
                    Graphic::Sampler Description;
                    Description.WrapModeU = Enum::Cast(Sampler.GetString("WrapModeU"), Graphic::TextureEdge::Clamp);
                    Description.WrapModeV = Enum::Cast(Sampler.GetString("WrapModeV"), Graphic::TextureEdge::Clamp);
                    Description.Filter    = Enum::Cast(Sampler.GetString("Filter"), Graphic::TextureFilter::MinMagMipPoint);

                    Asset->SetSampler(Semantic, Description);
                }
            }
        }

        // TODO: Parameter(s)
        return true;
    }
}
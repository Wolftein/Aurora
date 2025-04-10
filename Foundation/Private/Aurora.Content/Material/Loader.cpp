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

    Bool MaterialLoader::OnLoad(Ref<Service> Service, Any<Data> File, Ref<Graphic::Material> Asset)
    {
        TOMLParser        Parser(File.GetText());
        const TOMLSection Root     = Parser.GetSection("Material");
        const TOMLSection Textures = Parser.GetSection("Textures");
        const TOMLSection Samplers = Parser.GetSection("Samplers");

        // Parse 'properties' section
        Asset.SetKind(CastEnum(Root.GetString("Kind"), Graphic::Material::Kind::Normal));

        // Parse 'textures' section
        for (const Graphic::TextureSlot Slot : ListEnum<Graphic::TextureSlot>())
        {
            if (const TOMLSection Texture = Textures.GetSection(NameEnum(Slot), false); ! Texture.IsNull())
            {
                // Parse texture
                Asset.SetTexture(Slot, Service.Load<Graphic::Texture>(Texture.GetString("Path")));

                // Parse sampler
                if (const TOMLSection Sampler = Samplers.GetSection(Texture.GetString("Sampler")); ! Sampler.IsNull())
                {
                    Graphic::Sampler Description;
                    Description.EdgeU  = CastEnum(Sampler.GetString("Edge_U"), Graphic::TextureEdge::Clamp);
                    Description.EdgeV  = CastEnum(Sampler.GetString("Edge_V"), Graphic::TextureEdge::Clamp);
                    Description.Filter = CastEnum(Sampler.GetString("Filter"), Graphic::TextureFilter::Nearest);

                    Asset.SetSampler(Slot, Description);
                }
            }
        }

        // TODO: Parameter(s)

        return true;
    }
}
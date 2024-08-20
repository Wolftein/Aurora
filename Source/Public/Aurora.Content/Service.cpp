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

#include "Service.hpp"
#include "Locator/MemoryLocator.hpp"

#ifdef    AE_CONTENT_LOADER_MP3
    #include "Aurora.Content/Sound/MP3/Loader.hpp"
#endif // AE_CONTENT_LOADER_MP3
#ifdef    AE_CONTENT_LOADER_WAV
    #include "Aurora.Content/Sound/WAV/Loader.hpp"
#endif // AE_CONTENT_LOADER_WAV
#ifdef    AE_CONTENT_LOADER_STB
    #include "Aurora.Content/Texture/STB/Loader.hpp"
#endif // AE_CONTENT_LOADER_STB
#ifdef    AE_CONTENT_LOADER_EFFECT
    #include "Aurora.Content/Shader/Loader.hpp"
#endif // AE_CONTENT_LOADER_EFFECT
#ifdef    AE_CONTENT_LOADER_EFFECT
    #include "Aurora.Graphic/Service.hpp"
    #include "Aurora.Content/Pipeline/Loader.hpp"
#endif // AE_CONTENT_LOADER_EFFECT
#ifdef    AE_CONTENT_LOADER_ARTERY
    #include "Aurora.Content/Font/Artery/Loader.hpp"
#endif // AE_CONTENT_LOADER_ARTERY

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Service::Service(Ref<Context> Context)
        : Subsystem { Context }
    {
        RegisterDefaultResources();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnTick(UInt64 Time)
    {
        // TODO: Asynchronous
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::AddLoader(ConstSPtr<Loader> Loader)
    {
        for (const CStr Extension : Loader->GetExtensions())
        {
            mLoaders.try_emplace(Extension, Loader);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RemoveLoader(CStr Extension)
    {
        mLoaders.erase(Extension);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::AddLocator(CStr Schema, ConstSPtr<Locator> Locator)
    {
        mLocators.try_emplace(Schema, Locator);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RemoveLocator(CStr Schema)
    {
        mLocators.erase(Schema);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Data Service::Find(Ref<const Uri> Key)
    {
        if (const auto It = mLocators.find(Key.GetSchema()); It != mLocators.end())
        {
            return It->second->Read(Key.GetPath());
        }

        for (const auto Iterator : mLocators)
        {
            if (Data Data = Iterator.second->Read(Key.GetPath()); Data.HasData())
            {
                return Move(Data);
            }
        }
        return Data();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Service::Save(Ref<const Uri> Key, CPtr<const UInt8> Data)
    {
        if (const auto It = mLocators.find(Key.GetSchema()); It != mLocators.end())
        {
            It->second->Write(Key.GetPath(), Data);
            return true;
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Service::Delete(Ref<const Uri> Key)
    {
        if (const auto It = mLocators.find(Key.GetSchema()); It != mLocators.end())
        {
            It->second->Delete(Key.GetPath());
            return true;
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultResources()
    {
        AddLocator("Engine://", NewPtr<Content::MemoryLocator>());

        if (IsClientMode())
        {
#ifdef    AE_CONTENT_LOADER_MP3
            AddLoader(NewPtr<Content::MP3Loader>());
#endif // AE_CONTENT_LOADER_MP3

#ifdef    AE_CONTENT_LOADER_WAV
            AddLoader(NewPtr<Content::WAVLoader>());
#endif // AE_CONTENT_LOADER_WAV

#ifdef    AE_CONTENT_LOADER_STB
            AddLoader(NewPtr<Content::STBLoader>());
#endif // AE_CONTENT_LOADER_STB

#ifdef    AE_CONTENT_LOADER_EFFECT
            AddLoader(NewPtr<Content::ShaderLoader>());
#endif // AE_CONTENT_LOADER_EFFECT

#ifdef    AE_CONTENT_LOADER_EFFECT
            if (ConstSPtr<Graphic::Service> Graphics = GetSubsystem<Graphic::Service>())
            {
                Ref<const Graphic::Capabilities> GraphicCapabilities = Graphics->GetCapabilities();
                AddLoader(NewPtr<Content::PipelineLoader>(
                    GraphicCapabilities.Backend, GraphicCapabilities.Language));
            }
#endif // AE_CONTENT_LOADER_EFFECT

#ifdef    AE_CONTENT_LOADER_ARTERY
            AddLoader(NewPtr<Content::ArteryFontLoader>());
#endif // AE_CONTENT_LOADER_ARTERY
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Service::Parse(ConstSPtr<Resource> Asset)
    {
        Ref<const Uri> Key = Asset->GetKey();

        if (const auto Iterator = mLoaders.find(Key.GetExtension()); Iterator != mLoaders.end())
        {
            ConstSPtr<Loader> Loader = Iterator->second;

            if (Data File = Find(Key); File.HasData())
            {
                if (Loader->Load(* this, Move(File), * Asset))
                {
                    return true;
                }
                else
                {
                    Log::Warn("Resources: Failed to parse '{}'", Key.GetUrl());
                }
            }
            else
            {
                Log::Warn("Resources: '{}' not found.", Key.GetUrl());
            }
        }
        else
        {
            Log::Warn("Resources: Unknown file format '{}'", Key.GetExtension());
        }
        return false;
    }
}
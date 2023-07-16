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

#include "Service.hpp"

#ifdef    AE_CONTENT_LOCATOR_DEFAULT
    #include "Locator/SystemLocator.hpp"
#endif // AE_CONTENT_LOCATOR_DEFAULT
#ifdef    AE_CONTENT_LOADER_MP3
    #include "Content/Sound/MP3/Loader.hpp"
#endif // AE_CONTENT_LOADER_MP3
#ifdef    AE_CONTENT_LOADER_WAV
    #include "Content/Sound/WAV/Loader.hpp"
#endif // AE_CONTENT_LOADER_WAV
#ifdef    AE_CONTENT_LOADER_STB
    #include "Content/Texture/STB/Loader.hpp"
#endif // AE_CONTENT_LOADER_STB
#ifdef    AE_CONTENT_LOADER_EFFECT
    #include "Graphic/Service.hpp"
    #include "Content/Pipeline/Loader.hpp"
#endif // AE_CONTENT_LOADER_EFFECT
#ifdef    AE_CONTENT_LOADER_ARTERY
    #include "Content/Font/Artery/Loader.hpp"
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

    void Service::OnTick()
    {
        // TODO: async operation
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::AddLoader(Ref<const SPtr<Loader>> Loader)
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

    void Service::AddLocator(CStr Schema, Ref<const SPtr<Locator>> Locator)
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

    void Service::SetMemoryBudget(UInt Category, UInt Budget)
    {
        GetFactory(Category)->SetMemoryBudget(Budget);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UInt Service::GetMemoryBudget(UInt Category)
    {
        return GetFactory(Category)->GetMemoryBudget();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UInt Service::GetMemoryUsage(UInt Category)
    {
        return GetFactory(Category)->GetMemoryUsage();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Chunk Service::Find(Ref<const Uri> Key)
    {
        if (const auto It = mLocators.find(Key.GetSchema()); It != mLocators.end())
        {
            return It->second->Open(Key.GetPath());
        }

        for (const auto Iterator : mLocators)
        {
            if (Chunk Data = Iterator.second->Open(Key.GetPath()); Data.HasData())
            {
                return Move(Data);
            }
        }
        return Chunk();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Register(Ref<const SPtr<Resource>> Asset, Bool Async)
    {
        if (Asset)
        {
            const Bool Inserted = GetFactory(Asset->GetCategory())->Insert(Asset);

            if (Inserted)
            {
                if (Async)
                {
                    LOG_ERROR("Resources: Async operation is not yet supported, please use sync mode"); // TODO: async operation
                }
                else
                {
                    Process(Asset, true);
                }
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Load(Ref<const SPtr<Resource>> Asset, Bool Async)
    {
        if (Asset && Asset->HasCreated())
        {
            Asset->SetCondition(Resource::Condition::Pending);

            if (Async)
            {
                LOG_ERROR("Resources: Async operation is not yet supported, please use sync mode"); // TODO: async operation
            }
            else
            {
                Process(Asset, Parse(Asset));
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Reload(Ref<const SPtr<Resource>> Asset, Bool Async)
    {
        if (Asset && Asset->HasFinished())
        {
            if (Async)
            {
                LOG_ERROR("Resources: Async operation is not yet supported, please use sync mode"); // TODO: async operation
            }
            else
            {
                Process(Asset, Parse(Asset));
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Unload(Ref<const SPtr<Resource>> Asset)
    {
        if (Asset && GetFactory(Asset->GetCategory())->Unload(Asset->GetKey()))
        {
            Process(Asset, false);  // TODO: async operation
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Prune(UInt Category, Bool Force)
    {
        const Vector<SPtr<Resource>> Assets = GetFactory(Category)->Prune(Force);

        for (Ref<const SPtr<Resource>> Asset : Assets)
        {
            Process(Asset, false);  // TODO: async operation
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultResources()
    {
#ifdef    AE_CONTENT_LOCATOR_DEFAULT
        AddLocator("file://", NewPtr<Content::SystemLocator>());
#endif // AE_CONTENT_LOCATOR_DEFAULT

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
        Ref<const Graphic::Capabilities> GraphicCapabilities = GetSubsystem<Graphic::Service>()->GetCapabilities();
        AddLoader(NewPtr<Content::PipelineLoader>(
            GraphicCapabilities.Backend, GraphicCapabilities.Language));
#endif // AE_CONTENT_LOADER_EFFECT

#ifdef    AE_CONTENT_LOADER_ARTERY
        AddLoader(NewPtr<Content::ArteryFontLoader>());
#endif // AE_CONTENT_LOADER_ARTERY
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Ref<const SPtr<Factory>> Service::GetFactory(UInt Category)
    {
        if (auto Iterator = mFactories.find(Category); Iterator != mFactories.end())
        {
            return Iterator->second;
        }
        return mFactories.try_emplace(Category, NewPtr<Factory>()).first->second;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Service::Parse(Ref<const SPtr<Resource>> Asset)
    {
        Ref<const Uri> Key = Asset->GetKey();

        if (const auto Iterator = mLoaders.find(Key.GetExtension()); Iterator != mLoaders.end())
        {
            Ref<const SPtr<Loader>> Loader = Iterator->second;

            if (Chunk File = Find(Key); File.HasData())
            {
                if (Loader->Load(File, Asset))
                {
                    return true;
                }
                else
                {
                    LOG_WARNING("Resources: Failed to parse '{}'", Key.GetUrl());
                }
            }
            else
            {
                LOG_WARNING("Resources: '{}' not found.", Key.GetUrl());
            }
        }
        else
        {
            LOG_WARNING("Resources: Unknown file format '{}'", Key.GetExtension());
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::Process(Ref<const SPtr<Resource>> Asset, Bool Loaded)
    {
        Ref<const SPtr<Factory>> Factory = GetFactory(Asset->GetCategory());
        Ref<Context>             Context = GetContext();

        if (Loaded)
        {
            LOG_DEBUG("Resources: Loading '{}'", Asset->GetKey().GetUrl());

            const UInt64 LastMemoryUsage = Asset->GetMemory();

            // if the asset is being reloaded, then unload previous data from it
            // and load the new data
            if (Asset->HasLoaded())
            {
                Asset->OnUnload(Context);
            }

            if (Asset->OnLoad(Context))
            {
                Asset->SetCondition(Resource::Condition::Loaded);
            }
            else
            {
                Asset->SetCondition(Resource::Condition::Failed);
            }

            Factory->SetMemoryUsage(Factory->GetMemoryUsage() + Asset->GetMemory() - LastMemoryUsage);
        }
        else
        {
            LOG_DEBUG("Resources: Unloading '{}'", Asset->GetKey().GetUrl());

            Factory->SetMemoryUsage(Factory->GetMemoryUsage() - Asset->GetMemory());

            Asset->OnUnload(Context);
            Asset->SetCondition(Resource::Condition::Unloaded);
        }
    }
}
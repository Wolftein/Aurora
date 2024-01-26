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
    #include "Content/Sound/MP3/Loader.hpp"
#endif // AE_CONTENT_LOADER_MP3
#ifdef    AE_CONTENT_LOADER_WAV
    #include "Content/Sound/WAV/Loader.hpp"
#endif // AE_CONTENT_LOADER_WAV
#ifdef    AE_CONTENT_LOADER_STB
    #include "Content/Texture/STB/Loader.hpp"
#endif // AE_CONTENT_LOADER_STB
#ifdef    AE_CONTENT_LOADER_EFFECT
    #include "Content/Shader/Loader.hpp"
#endif // AE_CONTENT_LOADER_EFFECT
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

    void Service::AddLoader(ConstSPtr<Loader> Loader)
    {
        for (const CStr Extension : Loader->GetExtensions())
        {
            mLoaders.try_emplace(SStr(Extension), Loader);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RemoveLoader(CStr Extension)
    {
        mLoaders.erase(mLoaders.find(Extension));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::AddLocator(CStr Schema, ConstSPtr<Locator> Locator)
    {
        mLocators.try_emplace(SStr(Schema), Locator);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RemoveLocator(CStr Schema)
    {
        mLocators.erase(mLocators.find(Schema));
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

    void Service::Register(ConstSPtr<Resource> Asset, Bool Cacheable, Bool Async)
    {
        if (Asset)
        {
            const Bool Inserted = !Cacheable || GetFactory(Asset->GetCategory())->Insert(Asset);

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

    void Service::Load(ConstSPtr<Resource> Asset, Bool Async)
    {
        if (Asset && Asset->HasCreated())
        {
            Asset->SetStatus(Resource::Status::Pending);

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

    void Service::Reload(ConstSPtr<Resource> Asset, Bool Async)
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

    void Service::Unload(ConstSPtr<Resource> Asset)
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

        for (ConstSPtr<Resource> Asset : Assets)
        {
            Process(Asset, false);  // TODO: async operation
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultResources()
    {
        AddLocator("Engine://", NewPtr<Content::MemoryLocator>());

        if (! IsHeadless())
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
            Ref<const Graphic::Capabilities> GraphicCapabilities = GetSubsystem<Graphic::Service>()->GetCapabilities();
            AddLoader(NewPtr<Content::PipelineLoader>(
                GraphicCapabilities.Backend, GraphicCapabilities.Language));
#endif // AE_CONTENT_LOADER_EFFECT

#ifdef    AE_CONTENT_LOADER_ARTERY
            AddLoader(NewPtr<Content::ArteryFontLoader>());
#endif // AE_CONTENT_LOADER_ARTERY
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    ConstSPtr<Factory> Service::GetFactory(UInt Category)
    {
        if (auto Iterator = mFactories.find(Category); Iterator != mFactories.end())
        {
            return Iterator->second;
        }
        return mFactories.try_emplace(Category, NewPtr<Factory>()).first->second;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Service::Parse(ConstSPtr<Resource> Asset)
    {
        Ref<const Uri> Key = Asset->GetKey();

        if (const auto Iterator = mLoaders.find(Key.GetExtension()); Iterator != mLoaders.end())
        {
            ConstSPtr<Loader> Loader = Iterator->second;

            if (Chunk File = Find(Key); File.HasData())
            {
                if (Loader->Load(shared_from_this(), File, Asset))
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

    void Service::Process(ConstSPtr<Resource> Asset, Bool Loaded)
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
                Asset->Dispose(Context);
            }

            Asset->Create(Context);

            Factory->SetMemoryUsage(Factory->GetMemoryUsage() + Asset->GetMemory() - LastMemoryUsage);
        }
        else
        {
            LOG_DEBUG("Resources: Unloading '{}'", Asset->GetKey().GetUrl());

            Factory->SetMemoryUsage(Factory->GetMemoryUsage() - Asset->GetMemory());

            Asset->Dispose(Context);
        }
    }
}
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

#include "Service.hpp"
#include "Mount/EmbeddedMount.hpp"

#ifdef    AURORA_LOADER_MP3
    #include "Aurora.Content/Sound/MP3/Loader.hpp"
#endif // AURORA_LOADER_MP3
#ifdef    AURORA_LOADER_WAV
    #include "Aurora.Content/Sound/WAV/Loader.hpp"
#endif // AURORA_LOADER_WAV
#ifdef    AURORA_LOADER_STB
    #include "Aurora.Content/Texture/STB/Loader.hpp"
#endif // AURORA_LOADER_STB
#ifdef    AURORA_LOADER_MATERIAL
    #include "Aurora.Content/Material/Loader.hpp"
#endif // AURORA_LOADER_MATERIAL
#ifdef    AURORA_LOADER_GLTF
    #include "Aurora.Content/Model/GLTF/Loader.hpp"
#endif // AURORA_LOADER_GLTF
#ifdef    AURORA_LOADER_EFFECT
    #include "Aurora.Graphic/Service.hpp"
    #include "Aurora.Content/Pipeline/Loader.hpp"
#endif // AURORA_LOADER_EFFECT
#ifdef    AURORA_LOADER_ARTERY
    #include "Aurora.Content/Font/Artery/Loader.hpp"
#endif // AURORA_LOADER_ARTERY

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace Content
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Service::Service(Ref<Host> Host)
        : AbstractService { Host }
    {
        for (Ref<Thread> Worker : mLoaderThreads)
        {
            Worker = Thread(Capture(& Service::OnLoaderThread, this));
        }

        RegisterDefaultResources();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnTick(ConstRef<Time> Time)
    {
        AURORA_PROFILE_SCOPE("Content::Service::Tick");

        Lock<> Guard(mParserLatch);

        // Iterates through all pending scopes in the loader list.
        for (UInt32 Element = 0; Element < mParserList.size();)
        {
            // If the scope's dependencies are all resolved, finalize the associated resource.
            if (Ref<Scope> Scope = mParserList[Element]; Scope.Poll())
            {
                // Proceed to load the asset.
                OnAssetCreate(* Scope.GetResource());

                // Remove the completed scope from the list via swap-remove for efficiency.
                mParserList[Element] = Move(mParserList[mParserList.size() - 1]);
                mParserList.pop_back();
            }
            else
            {
                ++Element;
            }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnTeardown()
    {
        AURORA_PROFILE;

        for (Ref<Thread> Worker : mLoaderThreads)
        {
            Worker.request_stop();
        }

        {
            Lock<true> Guard(mLoaderLatch);
            mLoaderCondition.notify_all();
        }

        for (Ref<Thread> Worker : mLoaderThreads)
        {
            Worker.join();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::AddLoader(ConstTracker<Loader> Loader)
    {
        for (const ConstText Extension : Loader->GetExtensions())
        {
            mLoaders.try_emplace(Extension, Loader);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RemoveLoader(ConstText Extension)
    {
        mLoaders.erase(Extension);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::AddMount(ConstText Schema, ConstTracker<Mount> Mount)
    {
        mMounts.try_emplace(Schema, Mount);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RemoveMount(ConstText Schema)
    {
        mMounts.erase(Schema);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Blob Service::Find(ConstRef<Uri> Key)
    {
        if (const auto Iterator = mMounts.find(Key.GetSchema()); Iterator != mMounts.end())
        {
            return Iterator->second->Read(Key.GetPath());
        }

        for (const auto & [_, Mount] : mMounts)
        {
            if (Blob Data = Mount->Read(Key.GetPath()); Data != nullptr)
            {
                return Move(Data);
            }
        }
        return Blob();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Service::Save(ConstRef<Uri> Key, ConstSpan<Byte> Data)
    {
        if (const auto Iterator = mMounts.find(Key.GetSchema()); Iterator != mMounts.end())
        {
            Iterator->second->Write(Key.GetPath(), Data);
            return true;
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Service::Delete(ConstRef<Uri> Key)
    {
        if (const auto Iterator = mMounts.find(Key.GetSchema()); Iterator != mMounts.end())
        {
            Iterator->second->Delete(Key.GetPath());
            return true;
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnLoaderThread(ConstRef<std::stop_token> Token)
    {
        AURORA_THREAD_NAME("I/O Thread");

        LOG_INFO("Content: I/O Thread initialized.");

        while (! Token.stop_requested())
        {
            AURORA_PROFILE_SCOPE("Content::Service::Process");


            // Wait for new work or stop signal
            Scope Scope;

            {
                Lock<true> Guard(mLoaderLatch);

                mLoaderCondition.wait(Guard, [&]
                {
                    return !mLoaderList.empty() || Token.stop_requested();
                });

                if (Token.stop_requested() || mLoaderList.empty())
                {
                    continue;
                }

                Scope = std::move(mLoaderList.back());
                mLoaderList.pop_back();
            }

            // Do work outside lock
            if (OnLoaderParse(Scope))
            {
                Scope.GetResource()->SetStatus(Resource::Status::Failed);

                // Push parsed scope into parser list
                {
                    Lock<> Guard(mParserLatch);
                    mParserList.push_back(Move(Scope));
                }
            }
        }

        LOG_INFO("Content: I/O Thread stopped.");
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool Service::OnLoaderParse(Ref<Scope> Scope)
    {
        ConstRef<Uri> Key = Scope.GetResource()->GetKey();

        if (const auto Iterator = mLoaders.find(Key.GetExtension()); Iterator != mLoaders.end())
        {
            ConstTracker<Loader> Loader = Iterator->second;

            if (Blob File = Find(Key); File != nullptr)
            {
                if (Loader->Load(* this, Scope, Move(File)))
                {
                    return true;
                }
                LOG_WARNING("Content: Failed to parse '{}'", Key.GetUrl());
            }
            else
            {
                LOG_WARNING("Content: '{}' not found.", Key.GetUrl());
            }
        }
        else
        {
            LOG_WARNING("Content: Unknown file format '{}'", Key.GetExtension());
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnAssetLoad(ConstTracker<Resource> Asset, Ptr<Scope> Parent)
    {
        // If a parent Scope is provided, register this resource as a dependency of it.
        if (Parent)
        {
            Parent->Mark(Asset);
        }

        // Atomically transitions the resource's status from Idle to Queued.
        if (Asset->Transition(Resource::Status::Idle, Resource::Status::Queued))
        {
            // Queues the resource.
            {
                Lock<> Guard(mLoaderLatch);
                mLoaderList.emplace_back(Asset);
            }

            // Wake up one loader thread to process the resource
            mLoaderCondition.notify_one();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnAssetCreate(Ref<Resource> Asset)
    {
        LOG_DEBUG("Content: Loading '{}'", Asset.GetKey().GetUrl());

        Asset.Create(GetHost());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::OnAssetDelete(Ref<Resource> Asset)
    {
        LOG_DEBUG("Content: Unloading '{}'", Asset.GetKey().GetUrl());

        Asset.Delete(GetHost());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void Service::RegisterDefaultResources()
    {
        AddMount("Engine://", Tracker<EmbeddedMount>::Create());

        if (IsClientMode())
        {
#ifdef    AURORA_LOADER_MP3
            AddLoader(Tracker<MP3Loader>::Create());
#endif // AURORA_LOADER_MP3

#ifdef    AURORA_LOADER_WAV
            AddLoader(Tracker<WAVLoader>::Create());
#endif // AURORA_LOADER_WAV

#ifdef    AURORA_LOADER_STB
            AddLoader(Tracker<STBLoader>::Create());
#endif // AURORA_LOADER_STB

#ifdef    AURORA_LOADER_MATERIAL
            AddLoader(Tracker<MaterialLoader>::Create());
#endif // AURORA_LOADER_MATERIAL

#ifdef    AURORA_LOADER_GLTF
            AddLoader(Tracker<GLTFLoader>::Create());
#endif // AURORA_LOADER_GLTF

#ifdef    AURORA_LOADER_EFFECT
            Graphic::Capabilities Capabilities;

            if (ConstTracker<Graphic::Service> Graphics = GetService<Graphic::Service>())
            {
                Capabilities = Graphics->GetCapabilities();
            }

            AddLoader(Tracker<PipelineLoader>::Create(Capabilities.Backend, Capabilities.Language));
#endif // AURORA_LOADER_EFFECT
        }

#ifdef    AURORA_LOADER_ARTERY
        AddLoader(Tracker<ArteryFontLoader>::Create());
#endif // AURORA_LOADER_ARTERY
    }
}
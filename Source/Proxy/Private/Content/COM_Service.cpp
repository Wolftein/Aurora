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

#include "COM_Service.hpp"
#include "Audio/COM_Sound.hpp"
#include "Graphic/COM_Font.hpp"
#include "Graphic/COM_Material.hpp"
#include "Graphic/COM_Pipeline.hpp"
#include "Graphic/COM_Shader.hpp"
#include "Graphic/COM_Texture.hpp"
#include <Content/Locator/SystemLocator.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::FinalRelease()
    {
        mWrapper = nullptr;
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::AddLocator(vbStr8 Schema, IUnknown * Locator)
    {
        mWrapper->AddLocator(Schema, NewPtr<Visual_Basic_Content_Locator>(Locator));
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::AddSystemLocator(vbStr8 Schema, vbStr8 Folder)
    {
        mWrapper->AddLocator(Schema, NewPtr<Content::SystemLocator>(Folder));
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::RemoveLocator(vbStr8 Schema)
    {
        mWrapper->RemoveLocator(Schema);
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::SetMemoryBudget(Content_Resource_Type Category, vbInt64 Budget)
    {
        mWrapper->SetMemoryBudget(Category, VBInt64ToCPP(Budget));
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::GetMemoryBudget(Content_Resource_Type Category, vbInt64 * Result)
    {
        CPPToVBInt64(mWrapper->GetMemoryBudget(Category), * Result);
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::GetMemoryUsage(Content_Resource_Type Category, vbInt64 * Result)
    {
        CPPToVBInt64(mWrapper->GetMemoryUsage(Category), * Result);
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::Register(Content_Resource_ * Asset, vbBool Cacheable, vbBool Async)
    {
        mWrapper->Register(GetResource(Asset), VBIsTrue(Cacheable), VBIsTrue(Async));
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::Find(vbStr8 Key, Memory_Chunk_ ** Result)
    {
        (* Result) = CCreate<Memory_Chunk>(mWrapper->Find(Key));

        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::Retrieve(vbStr8 Key, Content_Resource_Type Category, vbBool CreateIfMissing, Content_Resource_ ** Result)
    {
#define RETRIEVE_RESOURCE(Base, Wrapper) \
        CCreate<Base>(mWrapper->Retrieve<Wrapper>(Content::Uri { Key }, VBIsTrue(CreateIfMissing)))

        switch (Category)
        {
        case Content_Resource_Type::eResourceTypeMaterial:
            (* Result) = RETRIEVE_RESOURCE(Graphic_Material, Graphic::Material);
            break;
        case Content_Resource_Type::eResourceTypePipeline:
            (* Result) = RETRIEVE_RESOURCE(Graphic_Pipeline, Graphic::Pipeline);
            break;
        case Content_Resource_Type::eResourceTypeSound:
            (* Result) = RETRIEVE_RESOURCE(Audio_Sound, Audio::Sound);
            break;
        case Content_Resource_Type::eResourceTypeTexture:
            (* Result) = RETRIEVE_RESOURCE(Graphic_Texture, Graphic::Texture);
            break;
        case Content_Resource_Type::eResourceTypeFont:
            (* Result) = RETRIEVE_RESOURCE(Graphic_Font, Graphic::Font);
            break;
        case Content_Resource_Type::eResourceTypeShader:
            (* Result) = RETRIEVE_RESOURCE(Graphic_Shader, Graphic::Shader);
            break;
        }
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::Load(vbStr8 Key, Content_Resource_Type Category, vbBool Async, Content_Resource_ ** Result)
    {
#define CREATE_RESOURCE(Base, Wrapper) \
        CCreate<Base>(mWrapper->Load<Wrapper>(Content::Uri { Key }, VBIsTrue(Async)))

        switch (Category)
        {
            case Content_Resource_Type::eResourceTypeMaterial:
                (* Result) = CREATE_RESOURCE(Graphic_Material, Graphic::Material);
                break;
            case Content_Resource_Type::eResourceTypePipeline:
                (* Result) = CREATE_RESOURCE(Graphic_Pipeline, Graphic::Pipeline);
                break;
            case Content_Resource_Type::eResourceTypeSound:
                (* Result) = CREATE_RESOURCE(Audio_Sound, Audio::Sound);
                break;
            case Content_Resource_Type::eResourceTypeTexture:
                (* Result) = CREATE_RESOURCE(Graphic_Texture, Graphic::Texture);
                break;
            case Content_Resource_Type::eResourceTypeFont:
                (* Result) = CREATE_RESOURCE(Graphic_Font, Graphic::Font);
                break;
            case Content_Resource_Type::eResourceTypeShader:
                (* Result) = CREATE_RESOURCE(Graphic_Shader, Graphic::Shader);
                break;
        }

        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::Reload(Content_Resource_ * Asset, vbBool Async)
    {
        mWrapper->Reload(GetResource(Asset), VBIsTrue(Async));
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::Unload(Content_Resource_ * Asset)
    {
        mWrapper->Unload(GetResource(Asset));
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::Exist(vbStr8 Key, Content_Resource_Type Category, vbBool * Result)
    {
        switch (Category)
        {
        case Content_Resource_Type::eResourceTypeMaterial:
            (* Result) = (mWrapper->Exist<Graphic::Material>(Key) ? VBTrue : VBFalse);
            break;
        case Content_Resource_Type::eResourceTypePipeline:
            (* Result) = (mWrapper->Exist<Graphic::Pipeline>(Key) ? VBTrue : VBFalse);
            break;
        case Content_Resource_Type::eResourceTypeSound:
            (* Result) = (mWrapper->Exist<Audio::Sound>(Key) ? VBTrue : VBFalse);
            break;
        case Content_Resource_Type::eResourceTypeTexture:
            (* Result) = (mWrapper->Exist<Graphic::Texture>(Key) ? VBTrue : VBFalse);
            break;
        case Content_Resource_Type::eResourceTypeFont:
            (* Result) = (mWrapper->Exist<Graphic::Font>(Key) ? VBTrue : VBFalse);
            break;
        case Content_Resource_Type::eResourceTypeShader:
            (* Result) = (mWrapper->Exist<Graphic::Shader>(Key) ? VBTrue : VBFalse);
            break;
        }

        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Content_Service::Prune(Content_Resource_Type Category, vbBool Force)
    {
        mWrapper->Prune(Category, VBIsTrue(Force));
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SPtr<Content::Resource> Content_Service::GetResource(Ptr<Content_Resource_> Asset)
    {
        Content_Resource_Type Type;
        Asset->GetCategory(& Type);

        switch (Type)
        {
        case Content_Resource_Type::eResourceTypeMaterial: return CCast<Graphic_Material>(Asset);
        case Content_Resource_Type::eResourceTypePipeline: return CCast<Graphic_Pipeline>(Asset);
        case Content_Resource_Type::eResourceTypeSound:    return CCast<Audio_Sound>(Asset);
        case Content_Resource_Type::eResourceTypeTexture:  return CCast<Graphic_Texture>(Asset);
        case Content_Resource_Type::eResourceTypeFont:     return CCast<Graphic_Font>(Asset);
        case Content_Resource_Type::eResourceTypeShader:   return CCast<Graphic_Shader>(Asset);
        }

        return nullptr;
    }
}
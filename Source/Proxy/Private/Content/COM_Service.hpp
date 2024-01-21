// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2024 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "COM_Locator.hpp"
#include "COM_Resource.hpp"
#include <Content/Service.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=(Undocumented)=-
    [object, uuid("4D805663-9766-494C-8CC3-411324C7C046"), pointer_default(unique)]
    __interface Content_Service_
    {
        // TODO: Loaders?

        HRESULT AddLocator([in] vbStr8 Schema, [in] IUnknown * Locator);

        HRESULT AddSystemLocator([in] vbStr8 Schema, [in] vbStr8 Folder);

        HRESULT RemoveLocator([in] vbStr8 Schema);

        HRESULT SetMemoryBudget([in] Content_Resource_Type Category, [in] vbInt64 Budget);

        HRESULT GetMemoryBudget([in] Content_Resource_Type Category, [out, retval] vbInt64 * Result);

        HRESULT GetMemoryUsage([in] Content_Resource_Type Category, [out, retval] vbInt64 * Result);

        HRESULT Register([in] Content_Resource_ * Asset, [in] vbBool Cacheable, [in, defaultvalue(false)] vbBool Async);

        HRESULT Find([in] vbStr8 Key, [out, retval] Memory_Chunk_ ** Result);

        HRESULT Retrieve([in] vbStr8 Key, [in] Content_Resource_Type Category, [in, defaultvalue(true)] vbBool CreateIfMissing, [out, retval] Content_Resource_ ** Result);

        HRESULT Load([in] vbStr8 Key, [in] Content_Resource_Type Category, [in, defaultvalue(false)] vbBool Async, [out, retval] Content_Resource_ ** Result);

        HRESULT Reload([in] Content_Resource_ * Asset, [in, defaultvalue(false)] vbBool Async);

        HRESULT Unload([in] Content_Resource_ * Asset);

        HRESULT Exist([in] vbStr8 Key, [in] Content_Resource_Type Category, [out, retval] vbBool * Result);

        HRESULT Prune([in] Content_Resource_Type Category, [in, defaultvalue(false)] vbBool Force);
    };

    // -=(Undocumented)=-
    [coclass, uuid("31D38D93-88C2-495C-AA2E-9F840C2170E2")]
    class ATL_NO_VTABLE Content_Service : public Content_Service_, public CSmartPtrWrapper<Content::Service>
    {
    public:

        // \inheritDoc
        HRESULT FinalRelease();

        // \see Content_Service_::AddLocator
        HRESULT AddLocator(vbStr8 Schema, IUnknown * Locator) override;

        // \see Content_Service_::AddSystemLocator
        HRESULT AddSystemLocator(vbStr8 Schema, vbStr8 Folder) override;

        // \see Content_Service_::RemoveLocator
        HRESULT RemoveLocator(vbStr8 Schema) override;

        // \see Content_Service_::SetMemoryBudget
        HRESULT SetMemoryBudget(Content_Resource_Type Category, vbInt64 Budget) override;

        // \see Content_Service_::GetMemoryBudget
        HRESULT GetMemoryBudget(Content_Resource_Type Category, vbInt64 * Result) override;

        // \see Content_Service_::GetMemoryUsage
        HRESULT GetMemoryUsage(Content_Resource_Type Category, vbInt64 * Result) override;

        // \see Content_Service_::Register
        HRESULT Register(Content_Resource_ * Asset, vbBool Cacheable, vbBool Async) override;

        // \see Content_Service_::Find
        HRESULT Find(vbStr8 Key, Memory_Chunk_ ** Result) override;

        // \see Content_Service_::Retrieve
        HRESULT Retrieve(vbStr8 Key, Content_Resource_Type Category, vbBool CreateIfMissing, Content_Resource_ ** Result) override;

        // \see Content_Service_::Load
        HRESULT Load(vbStr8 Key, Content_Resource_Type Category, vbBool Async, Content_Resource_ ** Result) override;

        // \see Content_Service_::Reload
        HRESULT Reload(Content_Resource_ * Asset, vbBool Async) override;

        // \see Content_Service_::Unload
        HRESULT Unload(Content_Resource_ * Asset) override;

        // \see Content_Service_::Exist
        HRESULT Exist(vbStr8 Key, Content_Resource_Type Category, vbBool * Result) override;

        // \see Content_Service_::Prune
        HRESULT Prune(Content_Resource_Type Category, vbBool Force) override;

    private:

        // -=(Undocumented)=-
        SPtr<Content::Resource> GetResource(Ptr<Content_Resource_> Asset);
    };
}
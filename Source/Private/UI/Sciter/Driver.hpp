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

#include "Input.hpp"
#include "UI/Driver.hpp"
#include "Content/Service.hpp"
#include "Graphic/Material.hpp"
#include "Graphic/Pipeline.hpp"
#include "Graphic/Renderer.hpp"
#include "Graphic/Service.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace UI
{
    // -=(Undocumented)=-
    class SciterDriver final : public Subsystem, public Driver
    {
    public:

        // -=(Undocumented)=-
        static constexpr UInt kMaxArguments = 16;

    public:

        // -=(Undocumented)=-
        SciterDriver(Ref<Subsystem::Context> Context);

        // -=(Undocumented)=-
        ~SciterDriver() override;

        // -=(Undocumented)=-
        Bool Initialise(UInt Width, UInt Height) override;

        // -=(Undocumented)=-
        void Reset(UInt Width, UInt Height) override;

        // -=(Undocumented)=-
        void Advance(Real64 Time) override;

        // -=(Undocumented)=-
        void Present() override;

        // -=(Undocumented)=-
        Bool Load(CStr Uri) override;

        // -=(Undocumented)=-
        Value Call(CStr Function, CPtr<const Value> Parameters) override;

        // -=(Undocumented)=-
        Value Eval(CStr Script) override;

        // -=(Undocumented)=-
        void Register(CStr Function, Callback OnExecute) override;

        // -=(Undocumented)=-
        void Unregister(CStr Function) override;

    private:

        // -=(Undocumented)=-
        void OnSciterPaint();

        // -=(Undocumented)=-
        void OnSciterLoad(LPSCN_LOAD_DATA Data);

        // -=(Undocumented)=-
        Bool OnSciterCall(Ptr<SCRIPTING_METHOD_PARAMS> Parameters);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        Bool                    mDirty;
        SPtr<Graphic::Renderer> mRenderer;
        SPtr<Graphic::Material> mMaterial;
        SPtr<Graphic::Pipeline> mPipeline;
        StringTable<Callback>   mRegistry;
    };
}
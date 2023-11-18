// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#pragma once

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "SciterInput.hpp"
#include <Content/Service.hpp>
#include <Input/Service.hpp>
#include <Graphic/Material.hpp>
#include <Graphic/Service.hpp>
#include <Platform/Service.hpp>
#include <Platform/Window.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace UI
{
    // -=(Undocumented)=-
    class SciterHost final : public Subsystem
    {
    public:

        // -=(Undocumented)=-
        using Function     = FPtr<sciter::value(CPtr<const sciter::value>)>;

    public:

        // -=(Undocumented)=-
        SciterHost(Ref<Context> Context);

        // -=(Undocumented)=-
        ~SciterHost();

        // \see Subsystem::OnTick
        void OnTick() override;

        // -=(Undocumented)=-
        Bool Initialise(ConstSPtr<Platform::Window> Window);

        // -=(Undocumented)=-
        Bool Load(CStr16 Uri);

        // -=(Undocumented)=-
        Bool Call(CStr16 Function, CPtr<const sciter::value> Parameters, Ptr<sciter::value> Return);

        // -=(Undocumented)=-
        Bool Eval(CStr16 Script, Ptr<sciter::value> Return);

        // -=(Undocumented)=-
        void CreateFunction(CStr Name, Function Method);

        // -=(Undocumented)=-
        void DeleteFunction(CStr Name);

        // -=(Undocumented)=-
        void SetVariable(CStr Name, Ref<const sciter::value> Value);

        // -=(Undocumented)=-
        sciter::value GetVariable(CStr Name);

        // -=(Undocumented)=-
        ConstSPtr<Graphic::Material> GetMaterial() const
        {
            return mMaterial;
        }

    private:

        // -=(Undocumented)=-
        void OnSciterLoad(LPSCN_LOAD_DATA Data);

        // -=(Undocumented)=-
        void OnSciterRender(LPCBYTE Bytes, INT X, INT Y, UINT Width, UINT Height);

        // -=(Undocumented)=-
        UInt OnSciterAttachBehaviour(LPSCN_ATTACH_BEHAVIOR Behavior);

        // -=(Undocumented)=-
        Bool OnSciterCall(Ptr<SCRIPTING_METHOD_PARAMS> Parameters);

    private:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Platform::Window>  mWindow;
        Bool                    mWindowIsDirty;
        StringTable<Function>   mFunctions;

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<Graphic::Material> mMaterial;
    };
}
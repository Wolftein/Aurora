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

#include "Graphic/COM_Material.hpp"
#include "SciterHost.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{

    // -=(Undocumented)=-
    [export, uuid("77919BFA-8989-11EE-ADDA-1418D3A8EDB8"), v1_enum]
    typedef enum Sciter_Function_Type
    {
        eSciterFunctionTypeProcedure,
        eSciterFunctionTypeFunction,
        eSciterFunctionTypeRender,
    } Sciter_Function_Type;

    // -=(Undocumented)=-
    [export, uuid("77919BFA-8989-11EE-ADDA-1418C3A8EDB8")]
    typedef struct Sciter_Render_Args
    {
        vbStr16  ID;
        vbReal32 X1;
        vbReal32 Y1;
        vbReal32 X2;
        vbReal32 Y2;
    } Sciter_Render_Args;

    // -=(Undocumented)=-
    [object, uuid("9B74E017-7A81-11EE-ADD5-1418C3A8EDB8"), pointer_default(unique)]
    __interface Sciter_Service_
    {
        HRESULT Present();

        HRESULT SetDocument([in] vbStr16 Uri, [out, retval] vbBool * Result);

        HRESULT SetStyle([in] vbStr16 Uri, [out, retval] vbBool * Result);

        [vararg]
        HRESULT Call([in] vbStr16 Function, [in, satype(vbVariant)] SAFEARRAY ** Parameters, [out, retval] vbVariant * Result);

        HRESULT Eval([in] vbStr16 Script, [out, retval] vbVariant * Result);

        HRESULT CreateFunction([in] vbStr16 Name, [in] Sciter_Function_Type Type, [in] vbInt32 Address);

        HRESULT DeleteFunction([in] vbStr16 Name);

        HRESULT SetVariable([in] vbStr16 Name, [in] vbVariant * Value);

        HRESULT GetVariable([in] vbStr16 Name, [out, retval] vbVariant * Value);

        [hidden] // HACK
        HRESULT _Fix_Export_Sciter_Render_Args([in] Sciter_Render_Args * Dummy);
    };

    // -=(Undocumented)=-
    [coclass, uuid("9D4A773A-7A81-11EE-ADD5-1418C3A8EDB8")]
    class ATL_NO_VTABLE Sciter_Service : public Sciter_Service_, public CSmartPtrWrapper<UI::SciterHost>
    {
    public:

        // \see Sciter_Service_::Present
        HRESULT Present();

        // \see Sciter_Service_::SetDocument
        HRESULT SetDocument(vbStr16 Uri, vbBool * Result) override;

        // \see Sciter_Service_::SetStyle
        HRESULT SetStyle(vbStr16 Uri, vbBool * Result) override;

        // \see Sciter_Service_::Call
        HRESULT Call(vbStr16 Function, SAFEARRAY ** Parameters, vbVariant * Result) override;

        // \see Sciter_Service_::Eval
        HRESULT Eval(vbStr16 Script, vbVariant * Result) override;

        // \see Sciter_Service_::CreateFunction
        HRESULT CreateFunction(vbStr16 Name, Sciter_Function_Type Type, vbInt32 Address) override;

        // \see Sciter_Service_::DeleteFunction
        HRESULT DeleteFunction(vbStr16 Name) override;

        // \see Sciter_Service_::SetVariable
        HRESULT SetVariable(vbStr16 Name, vbVariant * Value) override;

        // \see Sciter_Service_::GetVariable
        HRESULT GetVariable(vbStr16 Name, vbVariant * Value) override;

        // \see Sciter_Service_::_Fix_Export_Sciter_Render_Args
        HRESULT _Fix_Export_Sciter_Render_Args(Sciter_Render_Args * Dummy)
        {
            return S_OK;
        }
    };
}
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

#include "COM_SciterHost.hpp"

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace COM
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto CopyToValue(Ref<const vbVariant> Value)
    {
        const Bool ByReference = ((Value.vt & VT_BYREF) != 0);

        // TODO: Map, Object, Array
        switch (Value.vt & ~(VT_VECTOR | VT_ARRAY | VT_BYREF))
        {
        case VT_BOOL:
        {
            if (ByReference)
            {
                return sciter::value(VBIsTrue(* Value.pboolVal));
            }
            else
            {
                return sciter::value(VBIsTrue(Value.boolVal));
            }
        }
        case VT_I1:
        {
            if (ByReference)
            {
                return sciter::value(static_cast<SInt32>(* Value.pbVal));
            }
            else
            {
                return sciter::value(static_cast<SInt32>(Value.bVal));
            }
        }
        case VT_I2:
        {
            if (ByReference)
            {
                return sciter::value(static_cast<SInt32>(* Value.piVal));
            }
            else
            {
                return sciter::value(static_cast<SInt32>(Value.iVal));
            }
        }
        case VT_I4:
        {
            if (ByReference)
            {
                return sciter::value(static_cast<SInt32>(* Value.plVal));
            }
            else
            {
                return sciter::value(static_cast<SInt32>(Value.lVal));
            }
        }
        case VT_I8:
        {
            if (ByReference)
            {
                return sciter::value(static_cast<SInt32>(* Value.pllVal));
            }
            else
            {
                return sciter::value(static_cast<SInt32>(Value.llVal));
            }
        }
        case VT_UI1:
        {
            if (ByReference)
            {
                return sciter::value(static_cast<SInt32>(* Value.pbVal));
            }
            else
            {
                return sciter::value(static_cast<SInt32>(Value.bVal));
            }
        }
        case VT_UI2:
        {
            if (ByReference)
            {
                return sciter::value(static_cast<SInt32>(* Value.puiVal));
            }
            else
            {
                return sciter::value(static_cast<SInt32>(Value.uiVal));
            }
        }
        case VT_UI4:
        {
            if (ByReference)
            {
                return sciter::value(static_cast<SInt32>(* Value.pulVal));
            }
            else
            {
                return sciter::value(static_cast<SInt32>(Value.ulVal));
            }
        }
        case VT_UI8:
        {
            if (ByReference)
            {
                return sciter::value(static_cast<SInt32>(* Value.pullVal));
            }
            else
            {
                return sciter::value(static_cast<SInt32>(Value.ullVal));
            }
        }
        case VT_R4:
        {
            if (ByReference)
            {
                return sciter::value(* Value.pfltVal);
            }
            else
            {
                return sciter::value(Value.fltVal);
            }
        }
        case VT_R8:
        {
            if (ByReference)
            {
                return sciter::value(* Value.pdblVal);
            }
            else
            {
                return sciter::value(Value.dblVal);
            }
        }
        case VT_BSTR:
        {
            if (ByReference)
            {
                return sciter::value(* Value.pbstrVal, ::SysStringLen(* Value.pbstrVal));
            }
            else
            {
                return sciter::value(Value.bstrVal, ::SysStringLen(Value.bstrVal));
            }
        }
        /*
        case VT_CY:
        case VT_DECIMAL:
        {
            if (ByReference)
            {
                return sciter::value(Value.pcyVal->int64);
            }
            else
            {
                return sciter::value(Value.cyVal.int64);
            }
        }*/
        case VT_SAFEARRAY: // @NOTE: only support byte array for now
        {
            const LPSAFEARRAY Array = Value.parray;

            if (Array->cbElements == 0x01)
            {
                return sciter::value::make_bytes(
                    static_cast<Ptr<const sciter::byte>>(Array->pvData), Array->rgsabound[0].cElements);
            }
        }
        default:
        {
            LOG_WARNING("Invalid value type {}", Value.vt & ~(VT_VECTOR | VT_ARRAY | VT_BYREF));
            return sciter::value(static_cast<SInt32>(0));
        }
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto CopyToVariant(Ref<const sciter::value> Value)
    {
        vbVariant Return;

        // TODO: Map, Object, Array

        if (Value.is_bool())
        {
            Return.boolVal = (Value.get(false) ? VBTrue : VBFalse);
            Return.vt      = VT_BOOL;
        }
        else if (Value.is_int() | Value.is_big_int())
        {
            Return.lVal   = Value.get(0);
            Return.vt     = VT_UI4;
        }
        else if (Value.is_float())
        {
            Return.dblVal = Value.get(0.0);
            Return.vt     = VT_R8;
        }
        else if (Value.is_string())
        {
            Return.bstrVal = ::SysAllocString(Value.get(WSTR("")).data());
            Return.vt      = VT_BSTR;
        }
        else if (Value.is_bytes())
        {
            Ref<const aux::bytes> Bytes = Value.get_bytes();

            const LPSAFEARRAY SafeArrayPtr = ::SafeArrayCreateVector(VT_UI1, 0, Bytes.length);
            FastCopyMemory(SafeArrayPtr->pvData, Bytes.start, Bytes.length);

            Return.parray = SafeArrayPtr;
            Return.vt     = VT_ARRAY | VT_UI1;
        }
        else
        {
            Return.vt      = VT_NULL;
        }

        return Return;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto CallAssemblyFunction(UInt Address, CPtr<const sciter::value> Parameters)
    {
        vbVariant Result;

        if (Parameters.empty())
        {
            using Declaration = HRESULT (STDAPICALLTYPE *)(Ptr<vbVariant>);
            reinterpret_cast<Declaration>(Address)(& Result);
        }
        else
        {
            Vector<vbVariant> Values;
            Values.reserve(Parameters.size());

            for (Ref<const sciter::value> Parameter : Parameters)
            {
                Values.emplace_back(CopyToVariant(Parameter));
            }

            Ptr<SAFEARRAY> VarArguments;
            VBSpanToSafeArrayTemp(CPtr<vbVariant>(Values.data(), Values.size()), & VarArguments);

            using Declaration = HRESULT (STDAPICALLTYPE *)(Ptr<vbVariant>, Ptr<LPSAFEARRAY>);
            reinterpret_cast<Declaration>(Address)(& Result, & VarArguments);
        }
        return CopyToValue(Result);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto CallAssemblyProcedure(UInt Address, CPtr<const sciter::value> Parameters)
    {
        if (Parameters.empty())
        {
            using Declaration = HRESULT (STDAPICALLTYPE *)();
            reinterpret_cast<Declaration>(Address)();
        }
        else
        {
            Vector<vbVariant> Values;
            Values.reserve(Parameters.size());

            for (Ref<const sciter::value> Parameter : Parameters)
            {
                Values.emplace_back(CopyToVariant(Parameter));
            }

            Ptr<SAFEARRAY> VarArguments;
            VBSpanToSafeArrayTemp(CPtr<vbVariant>(Values.data(), Values.size()), & VarArguments);

            using Declaration = HRESULT (STDAPICALLTYPE *)(Ptr<LPSAFEARRAY>);
            reinterpret_cast<Declaration>(Address)(& VarArguments);
        }
        return sciter::value::nothing();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Sciter_Service::Present()
    {
        mWrapper->Present();
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Sciter_Service::SetDocument(vbStr16 Uri, vbBool * Result)
    {
        (* Result) = mWrapper->SetDocument(VBString16ToString16(Uri)) ? VBTrue : VBFalse;
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Sciter_Service::SetStyle(vbStr16 Uri, vbBool * Result)
    {
        (* Result) = mWrapper->SetStyle(VBString16ToString8(Uri));
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Sciter_Service::Call(vbStr16 Function, SAFEARRAY ** Parameters, vbVariant * Result)
    {
        Vector<sciter::value> SciterParameters;
        sciter::value         SciterResult;

        if (const CPtr<vbVariant> Array = VBSafeArrayToSpan<vbVariant>(Parameters); !Array.empty())
        {
            SciterParameters.reserve(Array.size());

            for (Ref<const vbVariant> Element : Array)
            {
                SciterParameters.emplace_back(CopyToValue(Element));
            }
        }

        if (mWrapper->Call(VBString16ToString16(Function), SciterParameters, & SciterResult))
        {
            (* Result) = CopyToVariant(SciterResult);
        }
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Sciter_Service::Eval(vbStr16 Script, vbVariant * Result)
    {
        sciter::value SciterReturn;

        if (mWrapper->Eval(VBString16ToString16(Script), & SciterReturn))
        {
            (* Result) = CopyToVariant(SciterReturn);
        }
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Sciter_Service::CreateFunction(vbStr16 Name, Sciter_Function_Type Type, vbInt32 Address)
    {
        switch (Type)
        {
        case eSciterFunctionTypeProcedure:
        {
            const auto OnTrampoline = [Address](CPtr<const sciter::value> Parameters) -> sciter::value
            {
                return CallAssemblyProcedure(Address, Parameters);
            };
            mWrapper->CreateFunction(VBString16ToString8(Name), OnTrampoline);
        }
        break;
        case eSciterFunctionTypeFunction:
        {
            const auto OnTrampoline = [Address](CPtr<const sciter::value> Parameters) -> sciter::value
            {
                return CallAssemblyFunction(Address, Parameters);
            };
            mWrapper->CreateFunction(VBString16ToString8(Name), OnTrampoline);
        }
        break;
        case eSciterFunctionTypeRender:
        {
            const auto OnTrampoline = [Address](CPtr<const sciter::value> Parameters) -> sciter::value
            {
                using Declaration = HRESULT (STDAPICALLTYPE *)(Ptr<Sciter_Render_Args>);

                Sciter_Render_Args Arguments;
                Arguments.ID = CopyToVariant(Parameters[0]).bstrVal;
                Arguments.X1 = CopyToVariant(Parameters[1]).lVal;
                Arguments.Y1 = CopyToVariant(Parameters[2]).lVal;
                Arguments.X2 = CopyToVariant(Parameters[3]).lVal;
                Arguments.Y2 = CopyToVariant(Parameters[4]).lVal;
                reinterpret_cast<Declaration>(Address)(& Arguments);

                return sciter::value::nothing();
            };
            mWrapper->CreateFunction(VBString16ToString8(Name), OnTrampoline);
        }
        break;
        }
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Sciter_Service::DeleteFunction(vbStr16 Name)
    {
        mWrapper->DeleteFunction(VBString16ToString8(Name));
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Sciter_Service::SetVariable(vbStr16 Name, vbVariant * Value)
    {
        mWrapper->SetVariable(VBString16ToString8(Name), Value ? CopyToValue(* Value) : sciter::value::nothing());
        return S_OK;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    HRESULT Sciter_Service::GetVariable(vbStr16 Name, vbVariant * Value)
    {
        (* Value) = CopyToVariant(mWrapper->GetVariable(VBString16ToString8(Name)));
        return S_OK;
    }
}
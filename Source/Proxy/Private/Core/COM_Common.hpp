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

#include <atldef.h>
#include <atlsafe.h>
#include <atlbase.h>
#include <atlcom.h>
#include <atlconv.h>
#include <atlctl.h>
#include <atlplus.h>
#include <Core/Core.hpp>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

inline namespace Proxy
{
    // -=(Undocumented)=-
    [export]
    typedef VARIANT         vbVariant;

    // -=(Undocumented)=-
    [export]
    typedef void *          vbAny;

    // -=(Undocumented)=-
    [export]
    typedef VARIANT_BOOL    vbBool;

    // -=(Undocumented)=-
    [export]
    typedef unsigned __int8 vbInt8;

    // -=(Undocumented)=-
    [export]
    typedef __int16         vbInt16;

    // -=(Undocumented)=-
    [export]
    typedef __int32         vbInt32;

    // -=(Undocumented)=-
    [export]
    typedef VARIANT         vbInt64;

    // -=(Undocumented)=-
    [export]
    typedef float           vbReal32;

    // -=(Undocumented)=-
    [export]
    typedef double          vbReal64;

    // -=(Undocumented)=-
    [export]
    typedef LPCSTR          vbStr8;

    // -=(Undocumented)=-
    [export]
    typedef BSTR            vbStr16;

    // -=(Undocumented)=-
#define VBTrue              VARIANT_TRUE

    // -=(Undocumented)=-
#define VBFalse             VARIANT_FALSE

    // -=(Undocumented)=-
    inline Bool VBIsTrue(vbBool Value)
    {
        return (Value == VBTrue);
    }

    // -=(Undocumented)=-
    inline Bool VBIsFalse(vbBool Value)
    {
        return (Value == VBFalse);
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline void CPPToVBInt64(Type Value, Ref<vbInt64> Output)
    {
        Output.vt    = VT_I8;
        Output.llVal = Value;
    }

    // -=(Undocumented)=-
    inline UInt64 VBInt64ToCPP(Ref<vbInt64> Input)
    {
        return Input.ullVal;
    }

    // -=(Undocumented)=-
    inline auto VBString16ToString8(vbStr16 Value)
    {
        const auto Length = SysStringLen(Value);

        SStr Result(WideCharToMultiByte(CP_UTF8, 0, Value, Length, nullptr, 0, nullptr, nullptr), '\0');
        WideCharToMultiByte(CP_UTF8, 0, Value, Length, Result.data(), Result.size(), nullptr, nullptr);
        return Result;
    }

    // -=(Undocumented)=-
    inline auto VBString8ToString16(CStr Value)
    {
        BSTR Result = SysAllocStringLen(
            nullptr, MultiByteToWideChar(CP_UTF8, 0, Value.data(), Value.size(), nullptr, 0));
        MultiByteToWideChar(CP_UTF8, 0, Value.data(), Value.size(), Result, Value.size());
        return Result;
    }

    // -=(Undocumented)=-
    inline auto VBString16ToString16(vbStr16 Value)
    {
        const auto Length = SysStringLen(Value);
        return CStr16(reinterpret_cast<Ptr<const char16_t>>(Value), Length);
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline auto VBSpanToSafeArrayTemp(CPtr<Type> Chunk, Ptr<LPSAFEARRAY> Result)
    {
        static SAFEARRAY GlobalUniqueArray;

        GlobalUniqueArray.cDims      = 1;
        GlobalUniqueArray.fFeatures  = FADF_AUTO | FADF_FIXEDSIZE;
        GlobalUniqueArray.cbElements = sizeof(Type);
        GlobalUniqueArray.cLocks     = 1;
        GlobalUniqueArray.pvData     = static_cast<LPVOID>(Chunk.data());

        GlobalUniqueArray.rgsabound[0].lLbound   = 0;
        GlobalUniqueArray.rgsabound[0].cElements = Chunk.size();

        (* Result) = & GlobalUniqueArray;
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline auto VBSafeArrayToSpan(SAFEARRAY * Data)
    {
        if (Data && SafeArrayGetDim(Data) > 0)
        {
            return CPtr<Type>(reinterpret_cast<Ptr<Type>>(Data->pvData), Data->rgsabound[0].cElements);
        }
        return CPtr<Type>();
    }

    // -=(Undocumented)=-
    template<typename Type>
    inline auto VBSafeArrayToSpan(SAFEARRAY ** Data)
    {
        return (Data ? VBSafeArrayToSpan<Type>(* Data) : CPtr<Type>());
    }

    // -=(Undocumented)=-
    template<typename T>
    class CObjectWrapper
    {
    public:

        // -=(Undocumented)=-
        void Internal_Reset(Ref<T> Reference)
        {
            mWrapper = Move(Reference);
        }

        // -=(Undocumented)=-
        auto Internal_Get()
        {
            return mWrapper;
        }

        // -=(Undocumented)=-
        auto Internal_Get_Move()
        {
            return Move(mWrapper);
        }

    protected:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        T mWrapper;
    };

    // -=(Undocumented)=-
    template<typename T>
    class CSmartPtrWrapper
    {
    public:

        // -=(Undocumented)=-
        CSmartPtrWrapper()
            : mWrapper { nullptr }
        {
        }

        // -=(Undocumented)=-
        void Internal_Reset(ConstSPtr<T> Pointer)
        {
            mWrapper = Pointer;
        }

        // -=(Undocumented)=-
        auto Internal_Get()
        {
            return mWrapper;
        }

    protected:

        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

        SPtr<T> mWrapper;
    };

    // -=(Undocumented)=-
    template<typename Base, typename Wrapper>
    static auto CCreate(Wrapper Instance)
    {
        CComObject<Base> * ComInstance;
        CComObject<Base>::CreateInstance(& ComInstance);
        ComInstance->Internal_Reset(Instance);
        ComInstance->AddRef();
        return ComInstance;
    }

    // -=(Undocumented)=-
    template<typename Base, typename Class>
    static auto CCast(Class Instance)
    {
        return reinterpret_cast<Ptr<Base>>(Instance)->Internal_Get();
    }

    // -=(Undocumented)=-
    template<typename Base, typename Class>
    static auto CMove(Class Instance)
    {
        return reinterpret_cast<Ptr<Base>>(Instance)->Internal_Get_Move();
    }
}
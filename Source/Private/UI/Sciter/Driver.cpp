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

#include "Driver.hpp"

#ifdef    EA_PLATFORM_WINDOWS
    #include <dxgi.h>
#endif // EA_PLATFORM_WINDOWS

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   DATA   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#define kHandle reinterpret_cast<HWINDOW>(1)

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace UI
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(Ref<const Value> Argument)
    {
        switch (Argument.index())
        {
        case 0:
            return sciter::value();
        case 1:
            return sciter::value(Argument.operator Bool());
        case 2:
            return sciter::value(Argument.operator SInt32());
        case 3:
            return sciter::value(Argument.operator Real());
        case 4:
            return sciter::value(Argument.operator SStr());
        default:
            return sciter::value(Argument.operator SInt32());
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    static auto As(Ref<const sciter::value> Argument)
    {
        Value Result;

        if (Argument.is_bool())
        {
            Result = Argument.get(false);
        }
        else if (Argument.is_int() || Argument.is_big_int())
        {
            Result = Argument.get(0);
        }
        else if (Argument.is_float())
        {
            Result = Argument.get(0.0);
        }
        else if (Argument.is_string() || Argument.is_error_string())
        {
            const aux::w2a String(Argument.get(L""));
            Result = SStr(String.c_str(), String.length());
        }
        return Result;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SciterDriver::SciterDriver(Ref<Subsystem::Context> Context)
        : Subsystem(Context)
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SciterDriver::~SciterDriver()
    {
        SciterProcX(kHandle, SCITER_X_MSG_DESTROY());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterDriver::Initialise(UInt Width, UInt Height)
    {
#ifdef    EA_PLATFORM_WINDOWS
        const GFX_LAYER kGraphicsBackend = GFX_LAYER_D2D;
#else
        const GFX_LAYER kGraphicsBackend = GFX_LAYER_SKIA;
#endif // EA_PLATFORM_WINDOWS

        // Enable default option(s)
#ifdef    EASTL_DEBUG
        SciterSetOption(kHandle, SCITER_SET_SCRIPT_RUNTIME_FEATURES,
                          ALLOW_FILE_IO
                        | ALLOW_SOCKET_IO
                        | ALLOW_EVAL
                        | ALLOW_SYSINFO);
        SciterSetOption(kHandle, SCITER_SET_DEBUG_MODE, true);
#else
        SciterSetOption(kHandle, SCITER_SET_SCRIPT_RUNTIME_FEATURES,
                          ALLOW_FILE_IO
                        | ALLOW_SOCKET_IO);
#endif // EASTL_DEBUG

        SciterSetOption(kHandle, SCITER_SET_GFX_LAYER, kGraphicsBackend);

        // Create sciter renderer
        if (!SciterProcX(kHandle, SCITER_X_MSG_CREATE(kGraphicsBackend, FALSE)))
        {
            return false;
        }

        // Create sciter callback
        constexpr auto OnHostCallback = [](LPSCITER_CALLBACK_NOTIFICATION Notification, LPVOID Parameter) -> UINT
        {
            const Ptr<SciterDriver> Host = static_cast<Ptr<SciterDriver>>(Parameter);

            switch(Notification->code)
            {
            case SC_LOAD_DATA:
                Host->OnSciterLoad(reinterpret_cast<LPSCN_LOAD_DATA>(Notification));
                break;
            case SC_INVALIDATE_RECT:
                Host->mDirty = true;
                break;
            }
            return 0;
        };
        SciterSetCallback(kHandle, OnHostCallback, this);

        // Create sciter element callback
        constexpr auto OnHostElementCallback = [](LPVOID Tag, HELEMENT Element, UINT Event, LPVOID Parameters) -> SBOOL
        {
            const Ptr<SciterDriver> Host = static_cast<Ptr<SciterDriver>>(Tag);

            if (Event == HANDLE_SCRIPTING_METHOD_CALL)
            {
                return Host->OnSciterCall(static_cast<Ptr<SCRIPTING_METHOD_PARAMS>>(Parameters));
            }
            return FALSE;
        };
        SciterWindowAttachEventHandler(kHandle, OnHostElementCallback, this, HANDLE_ALL);

        // Set device size and resolution (pixels per inch) TODO?
        SciterProcX(kHandle, SCITER_X_MSG_SIZE(Width, Height));
        SciterProcX(kHandle, SCITER_X_MSG_RESOLUTION(96));

        // Initialize the pipeline that will be used to render the ui
        mRenderer = NewPtr<Graphic::Renderer>(GetContext());
        mPipeline = GetSubsystem<Content::Service>()->Load<Graphic::Pipeline>("Engine://Pipeline/UI.effect");

        // Initialize the texture that will be used to render the ui
        const SPtr<Graphic::Texture> Texture = NewPtr<Graphic::Texture>(Content::Uri { "_SciterTexture" });
        Texture->Load(Graphic::TextureFormat::BGRA8UIntNorm, Graphic::TextureLayout::Dual, Width, Height, 1);
        Texture->Create(GetContext());

        // Initialize the material that will be used to render the ui
        mMaterial = NewPtr<Graphic::Material>(Content::Uri { "_SciterMaterial" });
        mMaterial->SetTexture(0, Texture);
        mMaterial->Create(GetContext());

        // Create the input listener and attach device event(s)
        GetSubsystem<Input::Service>()->AddListener(NewPtr<SciterInput>(kHandle));

        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterDriver::Reset(UInt Width, UInt Height)
    {
        ConstSPtr<Graphic::Texture> Texture = mMaterial->GetTexture(0);
        Texture->Dispose(GetContext());
        Texture->Load(Texture->GetFormat(), Texture->GetLayout(), Width, Height, Texture->GetLayer());
        Texture->Create(GetContext());

        SciterProcX(kHandle, SCITER_X_MSG_SIZE(Width, Height));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterDriver::Advance(Real64 Time)
    {
        SciterProcX(kHandle, SCITER_X_MSG_HEARTBIT(static_cast<UINT>(Time * 1000)));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterDriver::Present()
    {
        if (mDirty)
        {
            OnSciterPaint();
            mDirty = false;
        }

        ConstSPtr<Graphic::Texture> Texture = mMaterial->GetTexture(0);

        mRenderer->Begin(Matrix4f::CreateOrthographic(0, Texture->GetWidth(), Texture->GetHeight(), 0, 1.0f, -1.0f), 0.0f);
        mRenderer->DrawTexture(
            Rectf(0.0f, 0.0f, Texture->GetWidth(), Texture->GetHeight()),
            Rectf(0.0f, 0.0f, 1.0f, 1.0f), 0.0f, 0.0f, Graphic::Renderer::Order::Normal, -1, mPipeline, mMaterial);
        mRenderer->End();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterDriver::Load(CStr Uri)
    {
        const aux::a2w wUri(aux::slice(Uri.data(), Uri.size()));
        return SciterLoadFile(kHandle, wUri);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Value SciterDriver::Call(CStr Function, CPtr<const Value> Parameters)
    {
        Stack<sciter::value, kMaxArguments> lpParameters;
        for (Ref<const Value> Parameter : Parameters)
        {
            lpParameters.emplace_back(As(Parameter));
        }

        sciter::value lpReturn;
        SciterCall(kHandle, Function.data(), lpParameters.size(), lpParameters.data(), & lpReturn);

        if (lpReturn.is_error_string())
        {
            const aux::w2a Error(lpReturn.get(L""));
            LOG_WARNING("Sciter: Failed to call function '{}'", Error.c_str());
        }
        return As(lpReturn);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Value SciterDriver::Eval(CStr Script)
    {
        sciter::value  lpReturn;
        const aux::a2w wScript(aux::slice(Script.data(), Script.size()));

        const Bool Result = SciterEval(kHandle, wScript, Script.size(), & lpReturn);
        if (Result && lpReturn.is_error_string())
        {
            const aux::w2a Error(lpReturn.get(L""));
            LOG_WARNING("Sciter: Failed to eval script '{}'", Error.c_str());
        }
        return As(lpReturn);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterDriver::Register(CStr Function, Callback OnExecute)
    {
        mRegistry.try_emplace(SStr(Function), OnExecute);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterDriver::Unregister(CStr Function)
    {
        mRegistry.erase(SStr(Function));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterDriver::OnSciterPaint()
    {
        ConstSPtr<Graphic::Texture> Texture = mMaterial->GetTexture(0);

        SCITER_X_MSG_PAINT SciterPaintEvent;
        SciterPaintEvent.targetType = SPT_SURFACE;
#ifdef    EA_PLATFORM_WINDOWS
        SciterPaintEvent.target.pSurface
            = GetSubsystem<Graphic::Service>()->QueryTexture<Ptr<IDXGISurface>>(Texture->GetID());
#endif // EA_PLATFORM_WINDOWS
        SciterGetRootElement(kHandle, & SciterPaintEvent.element);
        SciterProcX(kHandle, SciterPaintEvent);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterDriver::OnSciterLoad(LPSCN_LOAD_DATA Data)
    {
        const SStr  Filename = aux::w2a(Data->uri).c_str();
        const Chunk Chunk    = GetSubsystem<Content::Service>()->Find(Content::Uri { Filename });

        if (Chunk.HasData())
        {
            LOG_INFO("Sciter: Loaded {}", Filename);
            ::SciterDataReady(kHandle, Data->uri, Chunk.GetData<UInt08>(), Chunk.GetSize());
        }
        else
        {
            LOG_WARNING("Sciter: Failed to load {}", Filename);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterDriver::OnSciterCall(Ptr<SCRIPTING_METHOD_PARAMS> Parameters)
    {
        Stack<Value, kMaxArguments> lpParameters;
        for (UInt Element = 0; Element < Parameters->argc; ++Element)
        {
            lpParameters.emplace_back(As(Parameters->argv[Element]));
        }

        if (const auto Iterator = mRegistry.find(Parameters->name); Iterator != mRegistry.end())
        {
            Parameters->result = As((Iterator->second)(lpParameters));
        }
        else
        {
            LOG_WARNING("Can't find function '{}'", Parameters->name);
        }
        return true;
    }
}
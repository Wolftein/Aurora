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

#include "SciterHost.hpp"
#include <dxgi.h>

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   DATA   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#define WINDOW_HANDLE reinterpret_cast<HWND>(1)

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [   CODE   ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

namespace UI
{
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SciterHost::SciterHost(Ref<Context> Context)
        : Subsystem { Context }
    {
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    SciterHost::~SciterHost()
    {
        SciterProcX(WINDOW_HANDLE, SCITER_X_MSG_DESTROY());
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterHost::OnTick()
    {
        // Execute Sciter's tick function
        SciterProcX(WINDOW_HANDLE, SCITER_X_MSG_HEARTBIT(GetSubsystem<Platform::Service>()->GetTime() * 1000));

        // Execute Sciter's draw function
        if (mDisplayIsDirty)
        {
            mDisplayIsDirty = false;

            // Draw function enclosed by our graphics device to be able to render custom element(s) using Aurora
            ConstSPtr<Graphic::Service> Graphics = GetSubsystem<Graphic::Service>();
            ConstSPtr<Graphic::Texture> Texture  = mMaterial->GetTexture(0);

            SCITER_X_MSG_PAINT SciterPaintEvent;
            SciterPaintEvent.targetType      = SPT_SURFACE;
            SciterPaintEvent.target.pSurface = Graphics->QueryTexture<Ptr<IDXGISurface>>(Texture->GetID());
            SciterGetRootElement(WINDOW_HANDLE, & SciterPaintEvent.element);

            const Vector2i Size = mDisplay->GetSize();
            const Rectf    Viewport(0.0f, 0.0f, Size.GetX(), Size.GetY());

            GetSubsystem<Graphic::Service>()->Prepare(mDevice, Viewport, Graphic::Clear::All, 0x00000000, 1.0f, 0);

            mRenderer->Begin(Matrix4f::CreateOrthographic(0.0f, Size.GetX(), Size.GetY(), 0.0f, 1.0f, -1.0f));
            {
                SciterProcX(WINDOW_HANDLE, SciterPaintEvent);
            }
            mRenderer->End();
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterHost::Initialise(ConstSPtr<Platform::Window> Display, ConstSPtr<Graphic::Renderer> Renderer)
    {
        // Attach the current window.
        mDisplay = Display;

        // Enable default option(s)
        SciterSetOption(WINDOW_HANDLE, SCITER_SET_SCRIPT_RUNTIME_FEATURES,
                          ALLOW_FILE_IO
                        | ALLOW_EVAL
                        | ALLOW_SOCKET_IO
                        | ALLOW_SYSINFO);
        SciterSetOption(WINDOW_HANDLE, SCITER_SET_GFX_LAYER, GFX_LAYER_D2D);

        // Create sciter renderer
        if (! SciterProcX(WINDOW_HANDLE, SCITER_X_MSG_CREATE(GFX_LAYER_D2D, FALSE)))
        {
            return false;
        }

        // Create sciter callback
        constexpr auto OnHostCallback = [](LPSCITER_CALLBACK_NOTIFICATION Notification, LPVOID Parameter) -> UINT
        {
            const Ptr<SciterHost> Host = static_cast<Ptr<SciterHost>>(Parameter);

            switch(Notification->code)
            {
            case SC_LOAD_DATA:
                Host->OnSciterLoad(reinterpret_cast<LPSCN_LOAD_DATA>(Notification));
                break;
            case SC_INVALIDATE_RECT:
                Host->mDisplayIsDirty = true;
                break;
            case SC_ATTACH_BEHAVIOR:
                return Host->OnSciterAttach(reinterpret_cast<LPSCN_ATTACH_BEHAVIOR>(Notification));
            }
            return 0;
        };
        SciterSetCallback(WINDOW_HANDLE, OnHostCallback, this);

        // Create sciter element callback
        constexpr auto OnHostElementCallback = [](LPVOID Tag, HELEMENT Element, UINT Event, LPVOID Parameters) -> SBOOL
        {
            const Ptr<SciterHost> Host = static_cast<Ptr<SciterHost>>(Tag);

            if (Event == HANDLE_SCRIPTING_METHOD_CALL)
            {
                return Host->OnSciterCall(static_cast<Ptr<SCRIPTING_METHOD_PARAMS>>(Parameters));
            }
            return FALSE;
        };
        SciterWindowAttachEventHandler(WINDOW_HANDLE, OnHostElementCallback, this, HANDLE_ALL);

        // Set device resolution (pixels per inch)
        const Vector2f Scale = Display->GetScale();
        SciterProcX(WINDOW_HANDLE, SCITER_X_MSG_RESOLUTION(Scale.GetX() * 96));

        // Set device size
        const Vector2i Size = Display->GetSize();
        SciterProcX(WINDOW_HANDLE, SCITER_X_MSG_SIZE(Size.GetX(), Size.GetY()));

        // Initialize the pipeline that will be used to render the ui
        mRenderer = Renderer;
        mPipeline = GetSubsystem<Content::Service>()->Load<Graphic::Pipeline>("Engine://Pipeline/UI.effect");

        // Initialize the texture that will be used to render the ui
        const SPtr<Graphic::Texture> Texture = NewPtr<Graphic::Texture>(Content::Uri { "_SciterTexture" });
        Texture->Load(Graphic::TextureFormat::BGRA8UIntNorm, Graphic::TextureLayout::Dual, Size.GetX(), Size.GetY(), 1);
        Texture->Create(GetContext());

        // Initialize the material that will be used to render the ui
        mMaterial = NewPtr<Graphic::Material>(Content::Uri { "_SciterMaterial" });
        mMaterial->SetTexture(0, Texture);
        mMaterial->Create(GetContext());

        // Initialize the pass that will be used to render the custom element(s).
        const Graphic::Object DepthTexture = GetSubsystem<Graphic::Service>()->CreateTexture(
            Graphic::TextureFormat::D24S8UIntNorm, Graphic::TextureLayout::Destination, Size.GetX(), Size.GetY(), 1);
        Array<Graphic::Object, 1> Colors {
            Texture->GetID()
        };
        mDevice = GetSubsystem<Graphic::Service>()->CreatePass(Colors, DepthTexture);

        // Create the input listener and attach window event(s)
        const auto OnDocumentResize = [this](SInt Width, SInt Height)
        {
            ConstSPtr<Graphic::Texture> Texture = mMaterial->GetTexture(0);

            Texture->Dispose(GetContext());
            Texture->Load(Texture->GetFormat(), Texture->GetLayout(), Width, Height, Texture->GetLayer());
            Texture->Create(GetContext());
        };
        GetSubsystem<Input::Service>()->AddListener(NewPtr<SciterInput>(WINDOW_HANDLE, OnDocumentResize));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterHost::Present()
    {
        const Vector2i Size = mDisplay->GetSize();

        const Rectf Destination { 0.0f, 0.0f, static_cast<Real32>(Size.GetX()), static_cast<Real32>(Size.GetY()) };
        const Rectf Source      { 0.0f, 0.0f, 1.0f, 1.0f };

        mRenderer->Begin(
            Matrix4f::CreateOrthographic(0, Size.GetX(), Size.GetY(), 0, 1.0f, -1.0f));
        mRenderer->DrawTexture(Destination, Source, 0.0f, 0.0f, Graphic::Renderer::Order::Normal, -1, mPipeline, mMaterial);
        mRenderer->End();
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterHost::SetDocument(CStr16 Uri)
    {
        return SciterLoadFile(WINDOW_HANDLE, reinterpret_cast<LPCWSTR>(Uri.data()));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterHost::SetStyle(CStr Uri)
    {
        const Chunk Chunk = GetSubsystem<Content::Service>()->Find(Content::Uri { Uri });

        if (Chunk.HasData())
        {
            return SciterSetMasterCSS(Chunk.GetData<const BYTE>(), Chunk.GetSize());
        }
        return false;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterHost::Call(CStr16 Function, CPtr<const sciter::value> Parameters, Ptr<sciter::value> Return)
    {
        aux::w2a FunctionName(reinterpret_cast<LPCWSTR>(Function.data()));

        const Bool Result = SciterCall(WINDOW_HANDLE, FunctionName, Parameters.size(), Parameters.data(), Return);
        if (Return->is_error_string())
        {
            const aux::w2a Error(Return->get(WSTR("")));
            LOG_WARNING("Sciter: Failed to call function '{}'", Error.c_str());
        }
        return Result;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterHost::Eval(CStr16 Script, Ptr<sciter::value> Return)
    {
        const Bool Result = SciterEval(WINDOW_HANDLE, reinterpret_cast<LPCWSTR>(Script.data()), Script.size(), Return);
        if (Return->is_error_string())
        {
            const aux::w2a Error(Return->get(WSTR("")));
            LOG_WARNING("Sciter: Failed to eval script '{}'", Error.c_str());
        }
        return Result;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterHost::CreateFunction(CStr Name, OnScriptCall Method)
    {
        mFunctions.try_emplace(SStr(Name), Method);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterHost::DeleteFunction(CStr Name)
    {
        mFunctions.erase(mFunctions.find(SStr(Name)));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterHost::SetVariable(CStr Name, Ref<const sciter::value> Value)
    {
        SciterSetVariable(WINDOW_HANDLE, Name.data(), std::addressof(Value));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    sciter::value SciterHost::GetVariable(CStr Name)
    {
        sciter::value Value;
        SciterGetVariable(WINDOW_HANDLE, Name.data(), std::addressof(Value));
        return Value;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UInt SciterHost::OnSciterAttach(LPSCN_ATTACH_BEHAVIOR Behavior)
    {
        constexpr auto OnElementProc = [](LPVOID Tag, HELEMENT Element, UINT Event, LPVOID Parameters) -> SBOOL
        {
            Ptr<SciterHost> Host = static_cast<Ptr<SciterHost>>(Tag);

            if (Event == HANDLE_DRAW)
            {
                return Host->OnSciterRender(Element, static_cast<Ptr<DRAW_PARAMS>>(Parameters));
            }
            return false;
        };

        if (strcmp(Behavior->behaviorName, "custom-widget") == 0)
        {
            Behavior->elementTag  = this;
            Behavior->elementProc = OnElementProc;
            return 1;
        }
        return 0;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    void SciterHost::OnSciterLoad(LPSCN_LOAD_DATA Data)
    {
        const CStr  Filename = aux::w2a(Data->uri).c_str();
        const Chunk Chunk    = GetSubsystem<Content::Service>()->Find(Content::Uri { Filename });

        if (Chunk.HasData())
        {
            LOG_INFO("Sciter: Loaded {}", Filename);
            ::SciterDataReady(WINDOW_HANDLE, Data->uri, Chunk.GetData<UInt08>(), Chunk.GetSize());
        }
        else
        {
            LOG_WARNING("Sciter: Failed to load {}", Filename);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterHost::OnSciterCall(Ptr<SCRIPTING_METHOD_PARAMS> Parameters)
    {
        if (const auto Iterator = mFunctions.find(Parameters->name); Iterator != mFunctions.end())
        {
            Parameters->result = (Iterator->second)(CPtr<const sciter::value>(Parameters->argv, Parameters->argc));
        }
        else
        {
            LOG_WARNING("Can't find function '{}'", Parameters->name);
        }
        return true;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterHost::OnSciterRender(HELEMENT Element, Ptr<DRAW_PARAMS> Parameters)
    {
        if(Parameters->cmd != DRAW_CONTENT)
        {
            return false;
        }

        sciter::string OnRenderCallbackAddress;
        SciterGetAttributeByNameCB(Element, "onRender", & _LPCWSTR2STRING, & OnRenderCallbackAddress);

        sciter::string ID;
        SciterGetAttributeByNameCB(Element, "id", & _LPCWSTR2STRING, & ID);

        Array<SCITER_VALUE, 5> Arguments;
        Arguments[0] = ID;
        Arguments[1] = sciter::value(Parameters->area.left);
        Arguments[2] = sciter::value(Parameters->area.top);
        Arguments[3] = sciter::value(Parameters->area.right);
        Arguments[4] = sciter::value(Parameters->area.bottom);

        SCRIPTING_METHOD_PARAMS Script;
        Script.name = aux::w2a(OnRenderCallbackAddress);
        Script.argc = Arguments.size();
        Script.argv = Arguments.data();

        return OnSciterCall(& Script);
    }
}
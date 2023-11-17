// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// [  HEADER  ]
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#include "SciterHost.hpp"
#include <Input/Service.hpp>

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
        SciterProcX(WINDOW_HANDLE, SCITER_X_MSG_HEARTBIT(UINT(GetSubsystem<Platform::Service>()->GetTime() * 1000)));

        // Execute Sciter's draw function
        if (mWindowIsDirty)
        {
            mWindowIsDirty = false;

            constexpr auto OnDrawCallback = [](LPCBYTE Bytes, INT X, INT Y, UINT Width, UINT Height, LPVOID Parameter)
            {
                static_cast<Ptr<SciterHost>>(Parameter)->OnSciterRender(Bytes, X, Y, Width, Height);
            };

            SCITER_X_MSG_PAINT SciterPaintEvent;
            SciterPaintEvent.targetType               = SPT_RECEIVER;
            SciterPaintEvent.target.receiver.param    = this;
            SciterPaintEvent.target.receiver.callback = OnDrawCallback;
            SciterGetRootElement(WINDOW_HANDLE, &SciterPaintEvent.element);
            SciterProcX(WINDOW_HANDLE, SciterPaintEvent);
        }
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterHost::Attach(ConstSPtr<Platform::Window> Window)
    {
        // Enable default option(s)
        SciterSetOption(WINDOW_HANDLE, SCITER_SET_SCRIPT_RUNTIME_FEATURES,
                          ALLOW_FILE_IO
                        | ALLOW_EVAL
                        | ALLOW_SOCKET_IO
                        | ALLOW_SYSINFO);

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
                Host->mWindowIsDirty = true;
                break;
            case SC_ATTACH_BEHAVIOR:
                return Host->OnSciterAttachBehaviour(reinterpret_cast<LPSCN_ATTACH_BEHAVIOR>(Notification));
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
        const Vector2f Scale = Window->GetScale();
        SciterProcX(WINDOW_HANDLE, SCITER_X_MSG_RESOLUTION(Scale.GetX() * 96));

        // Set device size
        const Vector2i Size = Window->GetSize();
        SciterProcX(WINDOW_HANDLE, SCITER_X_MSG_SIZE(Size.GetX(), Size.GetY()));

        // Initialize the texture that will be used to render the ui
        const SPtr<Graphic::Texture> Texture = NewPtr<Graphic::Texture>(Content::Uri { "_SciterTexture" });
        Texture->Load(Graphic::TextureFormat::BGRA8UIntNorm, Graphic::TextureLayout::Source, Size.GetX(), Size.GetY(), 1);
        Texture->Create(GetContext());

        // Initialize the material that will be used to render the ui
        mMaterial = NewPtr<Graphic::Material>(Content::Uri { "_SciterMaterial" });
        mMaterial->SetTexture(0, Texture);
        mMaterial->Create(GetContext());

        // Create the input listener and attach window event(s)
        const auto OnDocumentResize = [this](SInt Width, SInt Height)
        {
            ConstSPtr<Graphic::Texture> Texture = mMaterial->GetTexture(0);

            Texture->Dispose(GetContext());
            Texture->Load(Graphic::TextureFormat::BGRA8UIntNorm, Graphic::TextureLayout::Source, Width, Height, 1);
            Texture->Create(GetContext());
        };
        GetSubsystem<Input::Service>()->AddListener(NewPtr<SciterInput>(WINDOW_HANDLE, OnDocumentResize));
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterHost::Load(CStr16 Uri)
    {
        return SciterLoadFile(WINDOW_HANDLE, reinterpret_cast<LPCWSTR>(Uri.data()));
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

    void SciterHost::CreateFunction(CStr Name, Function Method)
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

    void SciterHost::OnSciterRender(LPCBYTE Bytes, INT X, INT Y, UINT Width, UINT Height)
    {
        const Graphic::Object ID   = mMaterial->GetTexture(0)->GetID();
        const CPtr<UInt08>    Data = CPtr<UInt08>(const_cast<Ptr<UInt08>>(Bytes), Width * Height * 4);

        GetSubsystem<Graphic::Service>()->UpdateTexture(ID, 0, Recti(X, Y, X + Width, Y + Height), Width * 4, Data);
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    UInt SciterHost::OnSciterAttachBehaviour(LPSCN_ATTACH_BEHAVIOR Behavior)
    {
        const Ptr<sciter::event_handler> Handler
            = sciter::behavior_factory::create(Behavior->behaviorName, Behavior->element);
        if (Handler)
        {
            Behavior->elementTag  = Handler;
            Behavior->elementProc = sciter::event_handler::element_proc;
            return 1;
        }
        return 0;
    }

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

    Bool SciterHost::OnSciterCall(Ptr<SCRIPTING_METHOD_PARAMS> Parameters)
    {
        const auto Iterator = mFunctions.find(Parameters->name);
        if (Iterator != mFunctions.end())
        {
            Parameters->result = (Iterator->second)(CPtr<const sciter::value>(Parameters->argv, Parameters->argc));
            return true;
        }
        return false;
    }
}
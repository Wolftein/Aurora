
#include <Windows.h>

#include <Content/Locator/SystemLocator.hpp>
#include <Content/Service.hpp>

#include <Audio/Service.hpp>

#include <Platform/Service.hpp>

#include <Input/Service.hpp>

#include <Graphic/Service.hpp>
#include <Graphic/Texture.hpp>
#include <Graphic/Pipeline.hpp>

#include <Content/Sound/WAV/Loader.hpp>
#include <Content/Sound/MP3/Loader.hpp>
#include <Content/Texture/STB/Loader.hpp>
#include <Content/Pipeline/Loader.hpp>

#include "Renderer.hpp"
#include <iostream>

void printFPS() {
    static std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();
    static int fps; fps++;

    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{ 1 }) {
        oldTime = std::chrono::high_resolution_clock::now();
        //  printf("FPS: %d\n", fps);
        fps = 0;
    }
}

// TODO: [Support Content Dependencies]
// TODO: [Cross Compilation]
// TODO: New Batch System
// TODO: New Font System
// TODO: New Camera System
// TODO: New UI System
// TODO: VB6 Proxy

int main()
{
    if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
    {
        return 0; // TODO_LOG: Cannot initialize OLE
    }

    Core::Subsystem::Context System;

    auto PlatformService = System.AddSubsystem<Platform::Service>();
    auto InputService = System.AddSubsystem<Input::Service>();

    auto Window = PlatformService->Initialise("Argentum Online v0.1", 2560, 1080);
    Vector2i Size = Window->GetSize();

    auto GraphicDriver  = Graphic::Backend::Direct3D11; // By default use D3D11
    auto GraphicService = System.AddSubsystem<Graphic::Service>();
    GraphicService->Initialise(GraphicDriver, Window->GetHandle(), Size.GetX(), Size.GetY());

    auto ContentService = System.AddSubsystem<Content::Service>();
    ContentService->AddLocator("file://", eastl::make_shared<Content::SystemLocator>());
    ContentService->AddLoader("wav", eastl::make_shared<Content::WAVLoader>());
    ContentService->AddLoader("mp3", eastl::make_shared<Content::MP3Loader>());

    auto ImgLoaderPtr = eastl::make_shared<Content::STBLoader>();
    ContentService->AddLoader("png", ImgLoaderPtr);
    ContentService->AddLoader("bmp", ImgLoaderPtr);
    ContentService->AddLoader("tga", ImgLoaderPtr);
    ContentService->AddLoader("psd", ImgLoaderPtr);
    ContentService->AddLoader("jpg", ImgLoaderPtr);

    //ContentService->AddLoader("hlsl", eastl::make_shared<HLSLLoader>());
    ContentService->AddLoader("pl", eastl::make_shared<Content::PipelineLoader>(Graphic::Backend::Direct3D11, Graphic::Language::Version_4_0));

   //     ContentService->AddLoader("hlsl", eastl::make_shared<HLSLLoader2>());


    constexpr UInt SUBMIX_MUSIC  = 0;
    constexpr UInt SUBMIX_EFFECT = 1;

    auto AudioDriver  = Audio::Backend::XAudio2; // By default use XAudio2
    auto AudioService = System.AddSubsystem<Audio::Service>();
    AudioService->Initialise(AudioDriver, 2);

    //SPtr<Audio::Sound> Sound1 = ContentService->Load<Audio::Sound>("85.wav", false);
    //SPtr<Audio::Sound> Sound2 = ContentService->Load<Audio::Sound>("127.mp3", false);

    AudioService->SetMasterVolume(1.0f);
    AudioService->SetSubmixVolume(SUBMIX_MUSIC, 1.0f);

    Vector3f NODE_POS { 30, 0, 71 };
    Vector3f LISTENER_POS { 30, 0, 70 };

    AudioService->SetSubmixVolume(SUBMIX_EFFECT, 0.55f);
    AudioService->SetListener(LISTENER_POS, {});
    SPtr<Audio::Emitter> Emitter = eastl::make_shared<Audio::Emitter>();
    Emitter->SetAttenuation(2.0f);
    Emitter->SetPosition(NODE_POS);

    //UInt sMusic = AudioService->Play(SUBMIX_MUSIC, Sound2, Emitter, true);
    //AudioService->Start(sMusic);

    SPtr<Graphic::Texture>  SimpleTexture  = ContentService->Load<Graphic::Texture>("rpgTile029.png", false);
    SPtr<Graphic::Pipeline> SimplePipeline;
    SimplePipeline = ContentService->Load<Graphic::Pipeline>("Water.pl", false);

    printf("Audio: %zukb\n", ContentService->GetMemoryUsage<Audio::Sound>() / 1024);
    printf("Texture: %zukb\n", ContentService->GetMemoryUsage<Graphic::Texture>() / 1024);
    printf("Pipeline: %zukb\n", ContentService->GetMemoryUsage<Graphic::Pipeline>() / 1024);

    SPtr<Renderer::Batch> Batcher = System.AddSubsystem<Renderer::Batch>();

    Real32 OffsetX = 0;
    Real32 OffsetY = 0;

    while (Window->Poll())
    {
        System.Run([](Ref<Core::Subsystem> Service)
        {
            Service.OnTick();
        });

        Graphic::Rect Viewport {
            0,
            0,
            static_cast<UInt16>(Size.GetX()),
            static_cast<UInt16>(Size.GetY())
        };

        if (InputService->IsKeyPressed(Input::Key::Escape))
        {
            Window->Close();
        }
        else if (InputService->IsKeyPressed(Input::Key::R))
        {
            ContentService->Reload(SimpleTexture, false);
            ContentService->Reload(SimplePipeline, false);
        }

        if (InputService->IsKeyHeld(Input::Key::Left))
        {
            OffsetX += 1;
        }
        if (InputService->IsKeyHeld(Input::Key::Right))
        {
            OffsetX -= 1;
        }
        if (InputService->IsKeyHeld(Input::Key::Up))
        {
            OffsetY += 1;
        }
        if (InputService->IsKeyHeld(Input::Key::Down))
        {
            OffsetY -= 1;
        }

        GraphicService->Prepare(Graphic::k_Default, Viewport, Graphic::Clear::All, { 0.0f }, 1.0f, 0);
        {
            Renderer::Batch::Rectangle Destination { 0 }, Source { 0, 0, 1, 1 };

            Batcher->SetData(OffsetX, OffsetY, Size.GetX(), Size.GetY(), PlatformService->GetTime());

            const UInt StepX = SimpleTexture->GetWidth() / 2;
            const UInt StepY = SimpleTexture->GetHeight() / 2;

            for (SInt X = 0; X < 4096; X += StepX)
            {
                for (SInt Y = 0; Y < 4096; Y += StepY)
                {
                    Destination.Left   = X;
                    Destination.Top    = Y;
                    Destination.Right  = Destination.Left + StepX;
                    Destination.Bottom = Destination.Top  + StepY;
                    Batcher->Draw(Destination, Source, false, 1, 0, 0xFFFFFFFF, SimplePipeline, SimpleTexture);
                }
            }

            Batcher->Flush();
        }
GraphicService->Commit(false);

printFPS();
}

return 0;
}
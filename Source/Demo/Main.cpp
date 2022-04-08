
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

Real32 DegToRad(Real32 Degrees) {
    return ( Degrees * 3.14 ) / 180;
}

Bool IsInsideTriangle(Ref<const Vector2f> A, Ref<const Vector2f> B, Ref<const Vector2f> C, Ref<const Vector2f> P)
{
    // Compute vectors
    Vector2f v0 = C - A;
    Vector2f v1 = B - A;
    Vector2f v2 = P - A;

    // Compute dot products
    Real32 dot00 = v0.Dot(v0);
    Real32 dot01 = v0.Dot(v1);
    Real32 dot02 = v0.Dot(v2);
    Real32 dot11 = v1.Dot(v1);
    Real32 dot12 = v1.Dot(v2);

    // Compute barycentric coordinates
    Real32 invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    Real32 u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    Real32 v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // Check if point is in triangle
    return (u >= 0 && v >= 0 && (u + v) < 1);
}

Bool IsInside(Ref<const Graphic::Camera> Camera, Ref<const Graphic::Rect> Viewport, Ref<const Vector2i> Mouse, Ref<const Renderer::Batch::Rectangle> Position)
{
    const Vector2f V1 = Camera.GetScreenCoordinates(Vector2f(Position.Left,  Position.Top), Viewport);
    const Vector2f V2 = Camera.GetScreenCoordinates(Vector2f(Position.Right, Position.Top), Viewport);
    const Vector2f V3 = Camera.GetScreenCoordinates(Vector2f(Position.Right, Position.Bottom), Viewport);
    const Vector2f V4 = Camera.GetScreenCoordinates(Vector2f(Position.Left,  Position.Bottom), Viewport);

    Vector2f Mousef(Mouse.GetX(), Mouse.GetY());

    if(IsInsideTriangle(V1,V2,V3,Mousef)) return true;
    if(IsInsideTriangle(V1,V3,V4,Mousef)) return true;
    return false;
}

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

    ContentService->AddLoader("pl", eastl::make_shared<Content::PipelineLoader>(Graphic::Backend::Direct3D11, Graphic::Language::Version_4_0));

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

    Graphic::Camera Camera;
    Camera.SetOrthographic(Size.GetX(), Size.GetY(), 1000, 0);

    Real32 Scaling = 1;
    Real32 Rotation = 0;

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
            Camera.Translate(Vector2f { 0.001, 0.0 });
        }
        if (InputService->IsKeyHeld(Input::Key::Right))
        {
            Camera.Translate(Vector2f { -0.001, 0.0 });
        }
        if (InputService->IsKeyHeld(Input::Key::Up))
        {
            Camera.Translate(Vector2f { 0, -0.001 });
        }
        if (InputService->IsKeyHeld(Input::Key::Down))
        {
            Camera.Translate(Vector2f { 0, +0.001 });
        }
        if (InputService->IsKeyHeld(Input::Key::A))
        {
            Camera.SetRotation(DegToRad(180), Vector3f(0, 0, 1));
        }
        if (InputService->IsKeyHeld(Input::Key::S))
        {
            Camera.SetRotation(DegToRad(90), Vector3f(0, 0, 1));
        }
        if (InputService->IsKeyHeld(Input::Key::D))
        {
            Camera.SetRotation(DegToRad(45), Vector3f(1, 1, 1));
        }
        if (InputService->IsKeyPressed(Input::Key::F1)) {

            auto Pos = Camera.GetScreenCoordinates(Vector3f(0, 0, 1), Viewport);
            printf("Screen %f %f %f\n", Pos.GetX(), Pos.GetY(), Pos.GetZ());
            Pos = Camera.GetScreenCoordinates(Vector3f(256, 0, 1), Viewport);
            printf("Screen %f %f %f\n", Pos.GetX(), Pos.GetY(), Pos.GetZ());
            Pos = Camera.GetScreenCoordinates(Vector3f(256, 256, 1), Viewport);
            printf("Screen %f %f %f\n", Pos.GetX(), Pos.GetY(), Pos.GetZ());
            Pos = Camera.GetScreenCoordinates(Vector3f(0, 256, 1), Viewport);
            printf("Screen %f %f %f\n", Pos.GetX(), Pos.GetY(), Pos.GetZ());

           // auto MousePos = InputService->GetMousePosition();
           // auto MousePosf = Vector3f(MousePos.GetX(), MousePos.GetY(), 0);
            //Pos = Camera.GetWorldCoordinates(MousePosf, Viewport);
            //printf("World %f %f %f\n", Pos.GetX(), Pos.GetY(), Pos.GetZ());

        }

        Scaling += InputService->GetMouseScroll().GetY() * 0.01;
        Camera.SetScale(Scaling, Scaling);
        Camera.Compute();

        auto MousePos = InputService->GetMousePosition();

        GraphicService->Prepare(Graphic::k_Default, Viewport, Graphic::Clear::All, { 0.0f }, 1.0f, 0);
        {
            Renderer::Batch::Rectangle Destination { 0 }, Source { 0, 0, 1, 1 };

            Batcher->SetData(Camera, PlatformService->GetTime());

            const UInt StepX = SimpleTexture->GetWidth();
            const UInt StepY = SimpleTexture->GetHeight();

            UInt Index = 0;

            for (SInt X = -4096; X < 16096; X += StepX)
            {
                for (SInt Y = -4096; Y < 16096; Y += StepY)
                {
                    UInt32 Color = 0xFFFFFFFF;
                    Destination.Left   = X;
                    Destination.Top    = Y;
                    Destination.Right  = Destination.Left + StepX;
                    Destination.Bottom = Destination.Top  + StepY;

                    if (IsInside(Camera, Viewport, MousePos, Destination))
                    {
                        Color = 0xFF0000FF;
                    }

                    Batcher->Draw(Destination, Source, false, 0, 0, Color, SimplePipeline, SimpleTexture);
                }
            }

            Batcher->Flush();
        }
GraphicService->Commit(false);

printFPS();
}

return 0;
}
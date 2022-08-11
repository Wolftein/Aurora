
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

#ifdef EA_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

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

Bool IsInside(Ref<const Graphic::Camera> Camera, Ref<const Rectf> Viewport, Ref<const Vector2i> Mouse, Ref<const Rectf> Position)
{
    const Vector2f V1 = Camera.GetScreenCoordinates(Vector2f(Position.GetLeft(),  Position.GetTop()), Viewport);
    const Vector2f V2 = Camera.GetScreenCoordinates(Vector2f(Position.GetRight(), Position.GetTop()), Viewport);
    const Vector2f V3 = Camera.GetScreenCoordinates(Vector2f(Position.GetRight(), Position.GetBottom()), Viewport);
    const Vector2f V4 = Camera.GetScreenCoordinates(Vector2f(Position.GetLeft(),  Position.GetBottom()), Viewport);

    Vector2f Mousef(Mouse.GetX(), Mouse.GetY());

    if(IsInsideTriangle(V1,V2,V3,Mousef)) return true;
    if(IsInsideTriangle(V1,V3,V4,Mousef)) return true;
    return false;
}

int main()
{
#ifdef EA_PLATFORM_WINDOWS
    if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
    {
        return 0; // TODO_LOG: Cannot initialize OLE
    }
#endif

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

    ContentService->AddLoader("effect", eastl::make_shared<Content::PipelineLoader>(Graphic::Backend::Direct3D11, Graphic::Language::Version_4));

    constexpr UInt SUBMIX_MUSIC  = 0;
    constexpr UInt SUBMIX_EFFECT = 1;

    auto AudioDriver  = Audio::Backend::XAudio2; // By default use XAudio2
    auto AudioService = System.AddSubsystem<Audio::Service>();
    AudioService->Initialise(AudioDriver, 2);

    //SPtr<Audio::Sound> Sound1 = ContentService->Load<Audio::Sound>("85.wav", false);
    //SPtr<Audio::Sound> Music = ContentService->Load<Audio::Sound>("127.mp3");

    AudioService->SetMasterVolume(1.0f);
    AudioService->SetSubmixVolume(SUBMIX_MUSIC, 1.0f);

    Vector3f NODE_POS { 30, 0, 71 };
    Vector3f LISTENER_POS { 30, 0, 70 };

    AudioService->SetSubmixVolume(SUBMIX_EFFECT, 0.55f);
    AudioService->SetListener(LISTENER_POS, {});
    SPtr<Audio::Emitter> Emitter = eastl::make_shared<Audio::Emitter>();
    Emitter->SetAttenuation(2.0f);
    Emitter->SetPosition(NODE_POS);

    //UInt MusicCue = AudioService->Play(SUBMIX_MUSIC, Music, Emitter, true);
    //AudioService->Start(MusicCue);

    SPtr<Graphic::Texture>  SimpleTexture  = ContentService->Load<Graphic::Texture>("rpgTile029.png");
    SPtr<Graphic::Pipeline> SimplePipeline = ContentService->Load<Graphic::Pipeline>("Water.effect");

    SPtr<Graphic::Renderer> Renderer = eastl::make_shared<Graphic::Renderer>(System);

    Graphic::Camera Camera;
    Camera.SetOrthographic(Size.GetX(), Size.GetY(), 1000, 0);

    Real32 Scaling = 1;
    Real32 Rotation = 0;

    SPtr<Graphic::Material> Material = eastl::make_shared<Graphic::Material>(Content::Uri { "Test"} ); // TODO: Loader
    Material->SetTexture(0, SimpleTexture);
    Material->SetParameter(0, Vector4f(1, 1, 0, 1));
    ContentService->Register(Material);

    SPtr<Graphic::Material> Material2 = eastl::make_shared<Graphic::Material>(Content::Uri { "Test_2" });
    Material2->SetTexture(0, SimpleTexture);
    Material2->SetParameter(0, Vector4f(1, 0, 0, 1));
    ContentService->Register(Material2);

    printf("Audio: %zu bytes\n", ContentService->GetMemoryUsage<Audio::Sound>());
    printf("Texture: %zu bytes\n", ContentService->GetMemoryUsage<Graphic::Texture>());
    printf("Pipeline: %zu bytes\n", ContentService->GetMemoryUsage<Graphic::Pipeline>());
    printf("Material: %zu bytes\n", ContentService->GetMemoryUsage<Graphic::Material>());

    while (Window->Poll())
    {
        System.Run([](Ref<Core::Subsystem> Service)
        {
            Service.OnTick();
        });

        Rectf Viewport {
            0,
            0,
            static_cast<Real32>(Size.GetX()),
            static_cast<Real32>(Size.GetY())
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

        Scaling += InputService->GetMouseScroll().GetY() * 0.01;
        Camera.SetScale(Scaling, Scaling);
        Camera.Compute();

        auto MousePos = InputService->GetMousePosition();


        GraphicService->Prepare(Graphic::k_Default, Viewport, Graphic::Clear::All, { 0.0f }, 1.0f, 0);
        {
            Rectf Destination, Source { 0, 0, 1, 1 };

            //Batcher->SetData(Camera, PlatformService->GetTime());
            Renderer->Begin(Camera, PlatformService->GetTime());

            const SInt StepX = SimpleTexture->GetWidth() / 2;
            const SInt StepY = SimpleTexture->GetHeight() / 2;

            UInt Index = 0;

            for (SInt X = -4096; X < 16000; X += StepX)
            {
                for (SInt Y = -4096; Y < 16000; Y += StepY)
                {
                    UInt32 Color = 0xFFFFFFFF;

                    Destination.Set(X, Y, X + StepX, Y + StepY);

                    if (IsInside(Camera, Viewport, MousePos, Destination))
                    {
                        Color = 0x30303030;
                    }

                    Renderer->Draw(
                        Destination,
                        Source,
                        0,
                        0,
                        Color,
                        SimplePipeline,
                        Index % 2 == 0 ? Material : Material2);
                    ++Index;
                }
            }

            Renderer->End();
        }
GraphicService->Commit(false);

printFPS();
}

return 0;
}
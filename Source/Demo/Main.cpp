
#include "Engine/Kernel.hpp"
#include "Experimental/Scene/Renderer.hpp"

// TODO: [Support Content Dependencies]
// TODO: [Profiling - Optik?]
// TODO: [Logging System]
// TODO: Rendering System
// TODO: Font System
// TODO: UI System
// TODO: VB6 Proxy

class Factory : public Core::Subsystem
{
public:
    Factory(Ref<Context> Context)
        : Subsystem { Context }
    {
        auto ContentService = Context.GetSubsystem<Content::Service>();
        SimpleTexture = ContentService->Load<Graphic::Texture>("rpgTile029.png");
        SimplePipeline = ContentService->Load<Graphic::Pipeline>("Water.effect");
        Renderer = NewPtr<Graphic::Renderer>(Context);

        Camera.SetOrthographic(800, 600, 1000, 0);

        SimpleMaterial = eastl::make_shared<Graphic::Material>(Content::Uri { "Test"} ); // TODO: Loader
        SimpleMaterial->SetTexture(0, SimpleTexture);
        SimpleMaterial->SetParameter(0, Vector4f(1, 1, 0, 1));
        ContentService->Register(SimpleMaterial);

    }

    void OnTick() override
    {

        Rectf Viewport {
            0,
            0,
            800.0f,
            600.0f
        };

        auto GraphicService = GetSubsystem<Graphic::Service>();
        auto PlatformService = GetSubsystem<Platform::Service>();
        auto InputService = GetSubsystem<Input::Service>();

        Scaling += InputService->GetMouseScroll().GetY() * 0.01;
        Camera.SetScale(Scaling, Scaling);
        Camera.Compute();

        GraphicService->Prepare(Graphic::k_Default, Viewport, Graphic::Clear::All, { 0.0f }, 1.0f, 0);
        {
            Renderer->Begin(Camera, PlatformService->GetTime());

            const SInt StepX = SimpleTexture->GetWidth() / 2;
            const SInt StepY = SimpleTexture->GetHeight() / 2;

            UInt Index = 0;

            Rectf Destination, Source { 0, 0, 1, 1 };

            for (SInt X = -4096; X < 8192; X += StepX)
            {
                for (SInt Y = -4096; Y < 8192; Y += StepY)
                {
                    UInt32 Color = 0xFFFFFFFF;

                    Destination.Set(X, Y, X + StepX, Y + StepY);

                    Renderer->Draw(
                        Destination,
                        Source,
                        0,
                        0,
                        Color,
                        SimplePipeline,
                        SimpleMaterial);
                    ++Index;
                }
            }
            Renderer->End();
        }
        GraphicService->Commit(false);
    }

private:
    SPtr<Graphic::Texture>  SimpleTexture;
    SPtr<Graphic::Pipeline> SimplePipeline;
    SPtr<Graphic::Material> SimpleMaterial;
    SPtr<Graphic::Renderer> Renderer;
    Graphic::Camera         Camera;
    Real32 Scaling = 1;
};

int main()
{
    Engine::Properties Properties;
    Properties.SetWindowTitle("Argentum Online v1.0");
    Properties.SetWindowWidth(800);
    Properties.SetWindowHeight(600);

    Engine::Kernel Kernel;
    Kernel.Initialize(Properties);
    Kernel.AddSubsystem<Factory>();
    Kernel.Run();

    return 0;
}
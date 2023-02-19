
#include "Engine/Kernel.hpp"
#include "Experimental/Scene/Renderer.hpp"

// TODO: [Support Content Dependencies]
// TODO: [Profiling - Optik?]
// TODO: [Logging System]
// TODO: Rendering System
// TODO: Font System
// TODO: UI System
// TODO: VB6 Proxy
void printFPS() {
    static std::chrono::time_point<std::chrono::steady_clock> oldTime = std::chrono::high_resolution_clock::now();
    static int fps; fps++;

    if (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime) >= std::chrono::seconds{ 1 }) {
        oldTime = std::chrono::high_resolution_clock::now();
        printf("FPS: %d\n", fps);
        fps = 0;
    }
}
static constexpr auto GAME_WINDOW_WIDTH  = 1280;
static constexpr auto GAME_WINDOW_HEIGHT = 1024;

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

        Camera.SetOrthographic(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, 1000, 0);

        SimpleMaterial = NewPtr<Graphic::Material>(Content::Uri { "Test"} ); // TODO: Loader
        SimpleMaterial->SetTexture(0, SimpleTexture);
        SimpleMaterial->SetParameter(0, Vector4f(1, 1, 0, 1));
        ContentService->Register(SimpleMaterial);

        SimpleMaterial2 = NewPtr<Graphic::Material>(Content::Uri { "Test #2"} ); // TODO: Loader
        SimpleMaterial2->SetTexture(0, SimpleTexture);
        SimpleMaterial2->SetParameter(0, Vector4f(1, 0, 0, 1));
        ContentService->Register(SimpleMaterial2);
    }

    void OnTick() override
    {

        Rectf Viewport {
            0,
            0,
            GAME_WINDOW_WIDTH,
            GAME_WINDOW_HEIGHT
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

            const SInt StepX = SimpleTexture->GetWidth();
            const SInt StepY = SimpleTexture->GetHeight();

            UInt Index = 0;

            Rectf Destination, Source { 0, 0, 1, 1 };

            for (SInt X = -4096; X < 4096; X += StepX)
            {
                for (SInt Y = -4096; Y < 4096; Y += StepY)
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
                        Index % 2 ? SimpleMaterial : SimpleMaterial2);
                    ++Index;
                }
            }
            Renderer->End();
        }
        GraphicService->Commit(Graphic::k_Default, false);
    }

private:
    SPtr<Graphic::Texture>  SimpleTexture;
    SPtr<Graphic::Pipeline> SimplePipeline;
    SPtr<Graphic::Material> SimpleMaterial;
    SPtr<Graphic::Material> SimpleMaterial2;
    SPtr<Graphic::Renderer> Renderer;
    Graphic::Camera         Camera;
    Real32 Scaling = 1;
};

int main()
{
    Engine::Properties Properties;
    Properties.SetWindowTitle("Argentum Online v1.0");
    Properties.SetWindowWidth(GAME_WINDOW_WIDTH);
    Properties.SetWindowHeight(GAME_WINDOW_HEIGHT);

    Engine::Kernel Kernel;
    Kernel.Initialize(Properties);
    Kernel.AddSubsystem<Factory>();
    Kernel.Run();

    return 0;
}
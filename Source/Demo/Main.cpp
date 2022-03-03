
#include <Windows.h>

#include <Content/Locator/RawLocator.hpp>
#include <Content/Service.hpp>

#include <Audio/Service.hpp>

#include <Platform/Service.hpp>

#include <Input/Service.hpp>

#include <Graphic/Service.hpp>
#include <Graphic/Texture.hpp>
#include <Graphic/Pipeline.hpp>

#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"
#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma comment( lib, "d3dcompiler" )
#include <d3dcompiler.h>
#include "Renderer.hpp"

class MP3Decoder : public Audio::Decoder
{
    constexpr static UInt BUFFER_SIZE = 44100 * 4;

public:

    MP3Decoder(Ref<Content::Reader> Input)
        : mData { eastl::move(Input) }
    {
        drmp3_init_memory(& mDescription, mData.GetData(), mData.GetRemaining(), NULL);
    }

    ~MP3Decoder() override {
    }

    virtual void Seek(UInt Seconds) override {
        drmp3_seek_to_start_of_stream(& mDescription);
    }
    virtual Bool Read(Ref<CPtr<UInt08>> Output) override
    {
        UInt Frames = drmp3_read_pcm_frames_f32(& mDescription, 44100, mSamples);

        Output = CPtr<UInt08>((UInt08 *) mSamples, Frames * mDescription.channels * sizeof(float));

        const Bool AtEnd = mDescription.atEnd;

        if (AtEnd)
        {
            drmp3_seek_to_start_of_stream(& mDescription);
        }
        return !AtEnd;
    }

    Content::Reader mData;
    drmp3  mDescription { 0 };
    Real32 mSamples[BUFFER_SIZE];
};

class WAVDecoder : public Audio::Decoder
{
public:

    WAVDecoder(Ref<Content::Reader> Input)
    {
        data = drwav_open_memory_and_read_pcm_frames_f32(Input.GetData(), Input.GetRemaining(), &channels, &sampleRate, &totalPCMFrameCount, NULL);
    }

    ~WAVDecoder() override {
        drwav_free(data, NULL);
    }

    virtual void Seek(UInt Seconds) override {}
    virtual Bool Read(Ref<CPtr<UInt08>> Output) override
    {
        Output = CPtr<UInt08>((UInt08 *) data, totalPCMFrameCount * channels * sizeof(float));
        return false;
    }

    float*       data;
    unsigned int channels;
    unsigned int sampleRate;
    drwav_uint64 totalPCMFrameCount;
};

class MP3Loader final : public Content::AbstractLoader<MP3Loader, Audio::Sound>
{
public:

    Bool Load(Ref<Content::Reader> Input, Ref<const SPtr<Audio::Sound>> Asset)
    {
        UPtr<MP3Decoder> Decoder = eastl::make_unique<MP3Decoder>(Input);
        Asset->Load(Decoder->mDescription.memory.dataSize, 32, Decoder->mDescription.channels, Decoder->mDescription.sampleRate, eastl::move(Decoder));

        return true;
    }
};

class WAVLoader final : public Content::AbstractLoader<WAVLoader, Audio::Sound>
{
public:

    Bool Load(Ref<Content::Reader> Input, Ref<const SPtr<Audio::Sound>> Asset)
    {
        UPtr<WAVDecoder> Decoder = eastl::make_unique<WAVDecoder>(Input);
        Asset->Load(Decoder->totalPCMFrameCount * Decoder->channels * 4, 32, Decoder->channels, Decoder->sampleRate, eastl::move(Decoder));

        return true;
    }
};

class IMGLoader final : public Content::AbstractLoader<IMGLoader, Graphic::Texture>
{
public:

    Bool Load(Ref<Content::Reader> Input, Ref<const SPtr<Graphic::Texture>> Asset)
    {
        SInt32 X, Y, Channel;

        Ptr<stbi_uc> Data = stbi_load_from_memory(Input.GetData(), Input.GetRemaining(), &X, &Y, &Channel, 4);
        if (Data)
        {
            Asset->Load(Graphic::TextureFormat::RGBA8UIntNorm, X, Y, 1, CPtr<UInt08> { Data, static_cast<UInt>(X * Y * 4) });

            stbi_image_free(Data);

            return true;
        }

        return false;
    }
};

class HLSLLoader final : public Content::AbstractLoader<HLSLLoader, Graphic::Pipeline>
    {
public:

    Bool Load(Ref<Content::Reader> Input, Ref<const SPtr<Graphic::Pipeline>> Asset)
    {
        Graphic::Descriptor Properties; // TODO: Parse all info
        Properties.InputLayout[0] = { 0, 0,  Graphic::VertexFormat::Float32x3   };
        Properties.InputLayout[1] = { 1, 12, Graphic::VertexFormat::UIntNorm8x4 };
        Properties.InputLayout[2] = { 2, 16, Graphic::VertexFormat::Float16x2   };

        Ptr<ID3DBlob> ES = nullptr;
        Ptr<ID3DBlob> VS = nullptr;
        Ptr<ID3DBlob> PS = nullptr;

        // TODO: Get Entry / Get Shader Model
        if (FAILED(D3DCompile(Input.GetData(), Input.GetRemaining(), nullptr, nullptr, nullptr,
                              "VSMain", "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, & VS, & ES )))
        {
            printf("%s\n", (char *) ES->GetBufferPointer());
            return false;
        }
        if (FAILED(D3DCompile(Input.GetData(), Input.GetRemaining(), nullptr, nullptr, nullptr,
                              "PSMain", "ps_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, & PS, & ES )))
        {
            VS->Release();
            printf("%s\n", (char *) ES->GetBufferPointer());
            return false;
        }

        CPtr<UInt08> CVS((UInt08 *) VS->GetBufferPointer(), VS->GetBufferSize());
        CPtr<UInt08> CPS((UInt08 *) PS->GetBufferPointer(), PS->GetBufferSize());

        Asset->Load(CVS, CPS, Properties);

        VS->Release();
        PS->Release();

        return true;
    }
};

int main()
{
    if (FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
    {
        return 0; // TODO_LOG: Cannot initialize OLE
    }

    Core::Subsystem::Context System;

    auto PlatformService = System.AddSubsystem<Platform::Service>();
    auto InputService = System.AddSubsystem<Input::Service>();

    auto Window = PlatformService->Initialise("Argentum Online v0.1", 1024, 768);
    Vector2i Size = Window->GetSize();

    auto GraphicDriver  = Graphic::Backend::Direct3D11; // By default use D3D11
    auto GraphicService = System.AddSubsystem<Graphic::Service>();
    GraphicService->Initialise(GraphicDriver, Window->GetHandle(), Size.GetX(), Size.GetY());

    auto ContentService = System.AddSubsystem<Content::Service>();
    ContentService->AddLocator("file://", eastl::make_shared<Content::RawLocator>());
    ContentService->AddLoader("wav", eastl::make_shared<WAVLoader>());
    ContentService->AddLoader("mp3", eastl::make_shared<MP3Loader>());

    auto ImgLoaderPtr = eastl::make_shared<IMGLoader>();
    ContentService->AddLoader("png", ImgLoaderPtr);
    ContentService->AddLoader("bmp", ImgLoaderPtr);
    ContentService->AddLoader("tga", ImgLoaderPtr);
    ContentService->AddLoader("psd", ImgLoaderPtr);
    ContentService->AddLoader("jpg", ImgLoaderPtr);

    if (GraphicDriver == Graphic::Backend::Direct3D11)
    {
        ContentService->AddLoader("sl", eastl::make_shared<HLSLLoader>());
    }

    constexpr UInt SUBMIX_MUSIC  = 0;
    constexpr UInt SUBMIX_EFFECT = 1;

    auto AudioService = System.AddSubsystem<Audio::Service>();
    AudioService->Initialise(2);

    SPtr<Audio::Sound> Sound1 = ContentService->Load<Audio::Sound>("Fuego.wav", false);
    SPtr<Audio::Sound> Sound2 = ContentService->Load<Audio::Sound>("Paradelous - Maverick Hunter (AMV).mp3", false);

    AudioService->SetMasterVolume(1.0f);
    AudioService->SetSubmixVolume(SUBMIX_MUSIC, 1.0f);

    Vector3f NODE_POS { 30, 0, 71 };
    Vector3f LISTENER_POS { 30, 0, 70 };

    AudioService->SetSubmixVolume(SUBMIX_EFFECT, 0.55f);
    AudioService->SetListener(LISTENER_POS, {});
    SPtr<Audio::Emitter> Emitter = eastl::make_shared<Audio::Emitter>();
    Emitter->SetAttenuation(2.0f);
    Emitter->SetPosition(NODE_POS);

    UInt sMusic = AudioService->Play(SUBMIX_MUSIC, Sound2, Emitter, true);
    //AudioService->Start(sMusic);

    SPtr<Graphic::Texture>  SimpleTexture  = ContentService->Load<Graphic::Texture>("Guerrero.jpg", false);
    SPtr<Graphic::Texture>  SimpleTexture2 = ContentService->Load<Graphic::Texture>("Pescador.jpg", false);
    SPtr<Graphic::Pipeline> SimplePipeline = ContentService->Load<Graphic::Pipeline>("basic.sl", false);

    printf("Audio: %zukb\n", ContentService->GetMemoryUsage<Audio::Sound>() / 1024);
    printf("Texture: %zukb\n", ContentService->GetMemoryUsage<Graphic::Texture>() / 1024);
    printf("Pipeline: %zukb\n", ContentService->GetMemoryUsage<Graphic::Pipeline>() / 1024);

    SPtr<Renderer::Batch> Batcher = System.AddSubsystem<Renderer::Batch>();

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
            ContentService->Reload(SimplePipeline, false);
            ContentService->Reload(SimpleTexture, false);
        }

        GraphicService->Prepare(Graphic::k_Default, Viewport, Graphic::Clear::All, { 0 }, 1.0f, 0);
        {
            Batcher->SetData(Size.GetX(), Size.GetY(), PlatformService->GetTime());

            Renderer::Batch::Rectangle Destination { 0 }, Source { 0, 0, 1, 1 };

            for (int i = 0; i < 1000; ++i)
            {
                Destination.Left   = i;
                Destination.Top    = i;
                Destination.Right  = Destination.Left + SimpleTexture->GetWidth();
                Destination.Bottom = Destination.Top + SimpleTexture->GetHeight();
                Batcher->Draw(Destination, Source, false, i * 25000, 0, 0xFFFFFFFF, SimplePipeline, SimpleTexture);
            }

            Batcher->Flush();
        }
        GraphicService->Commit(false);
    }

    return 0;
}
// Resources

Texture2D    ColorTexture : register(t0);
SamplerState ColorSampler : register(s0);

// Uniforms

cbuffer cb_Scene : register(b0)
{
    float4x4 uCamera;
};

// Definition

struct ps_Input
{
    float4 Position : SV_POSITION;
    float2 Texture  : TEXCOORD0;
    float4 Color    : COLOR0;
};

// VS Main

ps_Input vertex(float2 Position : POSITION, float2 Texture : TEXCOORD0, float4 Color : COLOR)
{
    ps_Input Result;
    Result.Position = mul(uCamera, float4(Position.xy, 0.f, 1.f));
    Result.Texture  = Texture;
    Result.Color    = Color;
    return Result;
}

// PS Main

float4 fragment(ps_Input Input) : SV_Target
{
    return Input.Color * ColorTexture.Sample(ColorSampler, Input.Texture);
}
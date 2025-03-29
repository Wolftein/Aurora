// Resources

Texture2D    ColorTexture : register(t0);
SamplerState ColorSampler : register(s0);

// Parameters

cbuffer cb_Scene : register(b0)
{
    float4x4 uCamera;
};

// Attributes

struct vs_Input
{
    float2 Position : POSITION;
    float2 Texture  : TEXCOORD0;
    float4 Color    : COLOR;
};

struct ps_Input
{
    float4 Position : SV_POSITION;
    float2 Texture  : TEXCOORD0;
    float4 Color    : COLOR0;
};

// VS Main

ps_Input vertex(vs_Input Input)
{
    ps_Input Result;
    Result.Position = mul(uCamera, float4(Input.Position.xy, 0.f, 1.f));
    Result.Texture  = Input.Texture;
    Result.Color    = Input.Color;
    return Result;
}

// PS Main

float4 fragment(ps_Input Input) : SV_Target
{
    float4 Texel = ColorTexture.Sample(ColorSampler, Input.Texture);

    clip(Texel.a == 0 ? -1 : +1);

    return Input.Color * Texel;
}
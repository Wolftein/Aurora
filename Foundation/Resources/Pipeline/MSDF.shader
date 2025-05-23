// Resources

Texture2D    ColorTexture : register(t0);
SamplerState ColorSampler : register(s0);

// Parameters

cbuffer cb_Global : register(b0)
{
    float4x4 u_Camera;
};

cbuffer cb_Material : register(b2)
{
    float2   u_Dimension;
    float    u_Distance;
};

// Attributes

struct vs_Input
{
    float3 Position : POSITION;
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
    Result.Position = mul(u_Camera, float4(Input.Position.xyz, 1.f));
    Result.Texture  = Input.Texture;
    Result.Color    = Input.Color;
    return Result;
}

// PS Main

float Median(float3 Color)
{
    return max(min(Color.r, Color.g), min(max(Color.r, Color.g), Color.b));
}

float4 fragment(ps_Input Input) : SV_Target
{
    const float3 Sample = ColorTexture.Sample(ColorSampler, Input.Texture).rgb;
    const float  Distance = Median(Sample) - 0.5;
    const float  DistanceAlpha = abs(ddx(Distance)) + abs(ddy(Distance));
    const float  Alpha = clamp(Distance / DistanceAlpha + 0.5, 0.0, 1.0);
    clip(Alpha <= 0.01 ? -1 : +1);
    return float4(Input.Color.rgb, Input.Color.a * Alpha);
}
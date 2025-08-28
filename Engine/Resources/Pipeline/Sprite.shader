// Resources

Texture2D    ColorTexture : register(t0);
SamplerState ColorSampler : register(s0);

// Parameters

cbuffer cb_Global : register(b0)
{
    float4x4 u_Camera;
};

// Attributes

struct vs_Input
{
    float3 Position  : POSITION;
    float2 Texture   : TEXCOORD0;
    float4 Color     : COLOR0;
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

    Result.Position = mul(u_Camera, float4(Input.Position, 1.f));
    Result.Texture  = Input.Texture;
    Result.Color    = Input.Color;

    return Result;
}

// PS Main

float4 fragment(ps_Input Input) : SV_Target
{
    float4 Texel = ColorTexture.Sample(ColorSampler, Input.Texture);

#ifdef    ENABLE_ALPHA_TEST
    clip(Texel.a - 0.001);
#endif // ENABLE_ALPHA_TEST

    return Input.Color * Texel;
}
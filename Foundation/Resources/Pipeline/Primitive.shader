// Parameters

cbuffer cb_Global : register(b0)
{
    float4x4 u_Camera;
};

// Attributes

struct vs_Input
{
    float3 Position : POSITION;
    float4 Color    : COLOR;
};

struct ps_Input
{
    float4 Position : SV_POSITION;
    float4 Color    : COLOR0;
};

// VS Main

ps_Input vertex(vs_Input Input)
{
    ps_Input Result;
    Result.Position = mul(u_Camera, float4(Input.Position.xyz, 1.f));
    Result.Color    = Input.Color;
    return Result;
}

// PS Main

float4 fragment(ps_Input Input) : SV_Target
{
    return Input.Color;
}
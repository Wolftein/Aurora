// #######################
//         UNIFORM       
// #######################

cbuffer UBScene    : register(b0)
{
    float4x4 uCamera;
}

// #######################
//       ATTRIBUTE
// #######################

struct VSInput
{
    float3 Position : ATTRIBUTE0;
    float4 Color    : ATTRIBUTE1;
};

struct FSInput
{
    float4 Position : SV_POSITION;
    float4 Color    : COLOR;
};

// #######################
//        PROGRAM
// #######################

FSInput vertex(VSInput Input)
{
    FSInput Output;

    Output.Position = mul(uCamera, float4(Input.Position,  1));
    Output.Color    = Input.Color;

    return Output;
}

float4 fragment(FSInput Input) : SV_Target
{
    return Input.Color;
}

// #######################
//         UNIFORM       
// #######################

cbuffer UBScene    : register(b0)
{
    float4x4 uCamera;
}

cbuffer UBMaterial : register(b2)
{
    float2   uDimension;
    float    uDistance;
}

// #######################
//       ATTRIBUTE
// #######################

struct VSInput
{
    float3 Position : ATTRIBUTE0;
    float4 Color    : ATTRIBUTE1;
    float2 Texture  : ATTRIBUTE2;
};

struct FSInput
{
    float4 Position : SV_POSITION;
    float4 Color    : COLOR;
    float2 Texture  : TEXCOORD0;
};

// #######################
//        PROGRAM
// #######################

// TODO: Configurable
#define OUTLINE_THICKNESS 0.4
#define OUTLINE_COLOR     float4(0, 0, 0, 1)

Texture2D    iChannel0          : register(s0);
SamplerState iChannel0Sampler;

FSInput vertex(VSInput Input)
{
    FSInput Output;

    Output.Position = mul(uCamera, float4(Input.Position,  1));
    Output.Color    = Input.Color;
    Output.Texture  = Input.Texture;

    return Output;
}

float Median(float3 Color)
{
    return max(min(Color.r, Color.g), min(max(Color.r, Color.g), Color.b));
}

float2 SafeNormalize(float2 Vector)
{
    float vLength = length(Vector);

    vLength = (vLength > 0.0) ? 1.0 / vLength : 0.0;

    return Vector * vLength;
}

float GetOpacityFromDistance(float signedDistance, float2 Jdx, float2 Jdy)
{
    const float distanceLimit = sqrt(2.0f) / 2.0f;  
    const float thickness = 1.0f / uDistance; 
    float2 gradientDistance = SafeNormalize(float2(ddx(signedDistance), ddy(signedDistance)));
    float2 gradient = float2(gradientDistance.x * Jdx.x + gradientDistance.y * Jdy.x, gradientDistance.x * Jdx.y + gradientDistance.y * Jdy.y);
    float scaledDistanceLimit = min(thickness * distanceLimit * length(gradient), 0.5f);
    return smoothstep(-scaledDistanceLimit, scaledDistanceLimit, signedDistance);
}

float4 fragment(VertexShaderOutput Input) : SV_Target
{
    float2 pixelCoord = Input.Texture * uDimension;
    float2 Jdx = ddx(pixelCoord);
    float2 Jdy = ddy(pixelCoord);
    float3 sample = iChannel0.Sample(iChannel0Sampler, Input.Texture).rgb;
    float medianSample = Median(sample);
    float signedDistance = medianSample - 0.5f;

    float strokeDistance = -(abs(medianSample - 0.25f - OUTLINE_THICKNESS) - OUTLINE_THICKNESS);

    float opacity = GetOpacityFromDistance(signedDistance, Jdx, Jdy);
    float strokeOpacity = GetOpacityFromDistance(strokeDistance, Jdx, Jdy);

    return lerp(OUTLINE_COLOR, Input.Color, opacity) * max(opacity, strokeOpacity);
}
// Resources

Texture2D    ColorTexture : register(t0);
SamplerState ColorSampler : register(s0);

// Parameters

cbuffer cb_Scene    : register(b0)
{
    float4x4 uCamera;
};

cbuffer cb_Material : register(b2)
{
    float2   uDimension;
    float    uDistance;
};

cbuffer cb_Instance : register(b3)
{
    float4   uOutlineColor;
    float    uOutlineThickness;
};

// Attributes

struct ps_Input
{
    float4 Position : SV_POSITION;
    float2 Texture  : TEXCOORD0;
    float4 Color    : COLOR0;
};

// VS Main

ps_Input vertex(float3 Position : POSITION, float2 Texture : TEXCOORD0, float4 Color : COLOR)
{
    ps_Input Result;
    Result.Position = mul(uCamera, float4(Position.xyz, 1.f));
    Result.Texture  = Texture;
    Result.Color    = Color;
    return Result;
}

// PS Main

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

float4 fragment(ps_Input Input) : SV_Target
{
    float2 pixelCoord = Input.Texture * uDimension;
    float2 Jdx = ddx(pixelCoord);
    float2 Jdy = ddy(pixelCoord);
    float3 sample = ColorTexture.Sample(ColorSampler, Input.Texture).rgb;
    float medianSample = Median(sample);
    float signedDistance = medianSample - 0.5f;

    float strokeDistance = -(abs(medianSample - 0.25f - uOutlineThickness) - uOutlineThickness);

    float opacity = GetOpacityFromDistance(signedDistance, Jdx, Jdy);
    float strokeOpacity = GetOpacityFromDistance(strokeDistance, Jdx, Jdy);

    return lerp(uOutlineColor, Input.Color, opacity) * max(opacity, strokeOpacity);
}
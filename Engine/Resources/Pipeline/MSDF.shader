// Resources

Texture2D    ColorTexture : register(t0);
SamplerState ColorSampler : register(s0);

struct PackedFontParameters // TODO: Investigate if all these parameters are required
{
    float  u_Rounded;               // 0.0 ... 1.0          (Bool)
    float  u_InvThreshold;          // 1.0 - Threshold
    float  u_OutlineBias;
    float  u_OutlineWidthAbsolute;
    float  u_OutlineWidthRelative;
    float  u_OutlineBlur;
    float4 u_OutlineColor;
};

// Parameters

cbuffer cb_Global   : register(b0)
{
    float4x4 u_Camera;
};

cbuffer cb_Material : register(b2)
{
    float2   u_Range;
};

cbuffer cb_Object   : register(b3)
{
    PackedFontParameters u_Parameters[128];
};

// Attributes

struct vs_Input
{
    float3 Position : POSITION;
    float2 Texture  : TEXCOORD0;
    float4 Color    : COLOR;
    uint   Instance : CUSTOM0;
};

struct ps_Input
{
    float4 Position : SV_POSITION;
    float2 Texture  : TEXCOORD0;
    float4 Color    : COLOR0;
    uint   Instance : CUSTOM0;
};

// VS Main

ps_Input vertex(vs_Input Input)
{
    ps_Input Result;
    Result.Position = mul(u_Camera, float4(Input.Position.xyz, 1.f));
    Result.Texture  = Input.Texture;
    Result.Color    = Input.Color;
    Result.Instance = Input.Instance;

    return Result;
}

// PS Main

float Median(float3 Color)
{
    return max(min(Color.r, Color.g), min(max(Color.r, Color.g), Color.b));
}

float Spread(float2 Coordinates, float2 Unit)
{
    return max(0.5 * dot(Unit, 1.0 / fwidth(Coordinates)), 1.0);
}

float4 fragment(ps_Input Input) : SV_Target
{
    const PackedFontParameters FontParameters = u_Parameters[Input.Instance];

    const float4 Sample       = ColorTexture.Sample(ColorSampler, Input.Texture);
    const float  DistanceSDF  = Sample.a;
    const float  DistanceMSDF = min(Median(Sample.rgb), DistanceSDF + 0.1);

    // Convert atlas distance into normalized screen-space range.
    const float  Scale = Spread(Input.Texture, u_Range);

    // Interpolated distance field depending on rounded vs sharp style.
    const float InnerStrokeDistance = lerp(DistanceMSDF, DistanceSDF, FontParameters.u_Rounded);
    const float OuterStrokeDistance = lerp(DistanceMSDF, DistanceSDF, FontParameters.u_Rounded);

    // Convert distance to alpha coverage.
    const float InnerStrokeA = Scale * (InnerStrokeDistance - FontParameters.u_InvThreshold) + 0.5 + FontParameters.u_OutlineBias;
    const float OuterStrokeA = Scale * (OuterStrokeDistance - FontParameters.u_InvThreshold + FontParameters.u_OutlineWidthRelative) + 0.5 + FontParameters.u_OutlineBias + FontParameters.u_OutlineWidthAbsolute;

    float4 InnerColor  = Input.Color;
    float4 OuterColor  = FontParameters.u_OutlineColor;
    float InnerOpacity = clamp(InnerStrokeA, 0.0, 1.0);
    float OuterOpacity = clamp(OuterStrokeA, 0.0, 1.0);

    // Optional: Apply Blur Effect
    if (FontParameters.u_OutlineBlur > 0.0)
    {
        const float BlurStart = FontParameters.u_OutlineWidthRelative + FontParameters.u_OutlineWidthAbsolute / Scale;
        OuterColor.a = smoothstep(
            BlurStart,
            BlurStart * (1.0 - FontParameters.u_OutlineBlur), FontParameters.u_InvThreshold - DistanceSDF - FontParameters.u_OutlineBias / Scale);
    }

    // Apply Gamma Correction.
    InnerOpacity = pow(InnerOpacity, 1.0 / 2.2);

    float4 Result = (InnerColor * InnerOpacity) + (OuterColor * (OuterOpacity - InnerOpacity));

#ifdef    ENABLE_ALPHA_TEST
    clip(Result.a - 0.001f);
#endif // ENABLE_ALPHA_TEST

    return Result;
}
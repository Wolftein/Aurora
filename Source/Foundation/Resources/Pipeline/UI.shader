// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Copyright (C) 2021-2023 by Agustin Alvarez. All rights reserved.
//
// This work is licensed under the terms of the MIT license.
//
// For a copy, see <https://opensource.org/licenses/MIT>.
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

Texture2D    ColorTexture : register(t0);
SamplerState ColorSampler : register(s0);

// Uniform

cbuffer SceneConstantBuffer : register(b0)
{
    float4x4 uCamera;
};

// Definition

struct PixelShaderInput
{
    float4 Position : SV_POSITION;
    float4 Color    : COLOR;
    float2 UV       : TEXCOORD0;
};

// VS Main

PixelShaderInput vertex(float3 Position : ATTRIBUTE0, float4 Color : ATTRIBUTE1, float2 UV : ATTRIBUTE2)
{
    PixelShaderInput Result;

    Result.Position = mul(uCamera, float4(Position,  1));
    Result.Color    = Color;
    Result.UV       = UV;

    return Result;
}

// PS Main

float4 fragment(PixelShaderInput Input) : SV_Target
{
    return Input.Color * ColorTexture.Sample(ColorSampler, Input.UV);
}
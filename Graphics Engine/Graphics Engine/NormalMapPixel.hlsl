#ifndef NormalMapPixel_HLSL
#define NormalMapPixel_HLSL

// === Shader Input Structures === //
struct PixelInput {
	float4 position : SV_POSITION;
	float4 worldPosition : WORLDPOS;
	float2 texCoords : UV;
	float4 normal : NORMALS;
	float4 tangent : TANGENT;
	float4 binormal : BINORMAL;
};
// =============================== //

// === Buffer Registers === //
cbuffer LIGHT_BUFFER : register(b0)
{
	// Directional
	float4 diffuseLightDirection : DF_DIRECTION;
	float4 diffuseLightColor : DF_COLOR;

	// Point
	float4 pointLightLocation : PT_LOCATION;
	float4 pointLightColor : PT_COLOR;

	// Spot
	float4 spotLightLocation : SP_LOCATION;
	float4 spotLightDirection : SP_DIRECTION;
	float4 spotLightColor : SP_COLOR;
	float4 spotLightConeRatio : SP_RATIO;
}
// ======================== //

// === Variable Registers
Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState filter : register(s0);

// === Main Function
float4 main(PixelInput _input) : SV_TARGET
{
	float4 textureColor;
	float4 normalMap;
	float3 normal;
	float lightIntensity;
	float4 color;

	// === Sample out the two textures
	textureColor = diffuseTexture.Sample(filter, _input.texCoords);
	normalMap = normalTexture.Sample(filter, _input.texCoords);

	// === Convert from (0 - +1) range to (-1 - +1) range
	normalMap = (normalMap * 2.0f) - 1.0f;

	// === Calculate the Normal
	float3x3 TBNMatrix = float3x3((float3)_input.tangent, (float3)_input.binormal, (float3)_input.normal);
	normal = mul(normalMap, TBNMatrix);
	normal = normalize(normal);

	// === Handle Directional Lighting
	lightIntensity = saturate(dot(normal, -diffuseLightDirection));
	color = saturate(diffuseLightColor * lightIntensity);
	color = color * textureColor;
	color.w = 1.0;

	return color;
}
// ===

#endif
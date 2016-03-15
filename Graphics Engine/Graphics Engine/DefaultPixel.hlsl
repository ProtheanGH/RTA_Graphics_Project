//////////////////////////////////////////////////////////////////////////
// DefaultPixel.hlsl
// Description:	Nothing special about this shader. Just the default.
// 
// Created By:		Doug Berg
// Last Edited:		3.9.2016
//////////////////////////////////////////////////////////////////////////
#ifndef DEFAULT_PIXEL_HLSL
#define DEFAULT_PIXEL_HLSL

// === Shader Input Structures === //
struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float4 worldPosition : WORLDPOS;
	float4 normals : NORMALS;
	float2 uv : UV;
};
// ===

// === Additional Structures === //
struct AmbientLight
{
	float4 color;
};

struct DirectionalLight
{
	float4 direction;
	float4 color;
};

struct PointLight
{
	float4 position;
	float4 color;
	float radius;

	float3 padding;
};

struct SpotLight
{
	float4 position;
	float4 direction;
	float4 color;
	float coneRatio;
	float radius;

	float3 padding;
};
// ============================= //

// === BUFFERS === //
cbuffer LIGHT_BUFFER : register(b0)
{
	AmbientLight ambientLight;
	DirectionalLight directionalLight;
	PointLight pointLight;
	SpotLight spotLight;
}
// ===

texture2D image : register(t0);

SamplerState filter : register(s0);

// === Main Function
float4 main( OUTPUT_VERTEX _input ) : SV_TARGET
{
	float4 imageColor = image.Sample(filter, _input.uv);

	// === Directional Light === //
	float directionRatio = saturate(dot(-diffuseLightDirection, _input.normals));
	float4 directionalResult = directionRatio * diffuseLightColor * imageColor;
	directionalResult.w = 1;

	// Ambient lighting
	float4 ambientDirection = imageColor * directionResult;


	// === Create a Greyscale === //
	float4 greyScale = { 0.25f, 0.25f, 0.25f, 1.0f };
	greyScale = greyScale * imageColor;
	// ===

	return saturate(greyScale + directionResult + ambientDirection);
}
// ===

#endif	// DEFAULT_PIXEL_HLSL


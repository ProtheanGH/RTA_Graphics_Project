//////////////////////////////////////////////////////////////////////////
// DefaultPixel.hlsl
// Description:	Nothing special about this shader. Just the default.
// 
// Created By:		Doug Berg
// Last Edited:		3.15.2016
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
// =============================== //

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
	float2 coneRatio;

	float2 padding;
};
// ============================= //

// === Buffer Registers === //
cbuffer LIGHT_BUFFER : register(b0)
{
	AmbientLight ambientLight;
	DirectionalLight directionalLight;
	PointLight pointLight;
	SpotLight spotLight;
}
// ======================== //

texture2D image : register(t0);

SamplerState filter : register(s0);

// === Main Function
float4 main(OUTPUT_VERTEX _input) : SV_TARGET
{
	float4 imageColor = image.Sample(filter, _input.uv);

	// === Directional Light === //
	float directionRatio = saturate(dot(-directionalLight.direction, _input.normals));
	float4 directionResult = directionRatio * directionalLight.color * imageColor;
	directionResult.w = 1;
	// ===


	// === Ambient lighting === //
	float4 ambientDirection = imageColor * directionResult;
	// ===


	// === Point Light === //
	float4 pointDirection = normalize(pointLight.position - _input.worldPosition);
	float  pointRatio = saturate(dot(pointDirection, _input.normals));
	float pointAtten = 1.0f - saturate(length(pointLight.position - _input.worldPosition) / pointLight.radius);
	pointAtten *= pointAtten;
	float4 pointResult = pointAtten * pointRatio * pointLight.color * imageColor;
	pointResult.w = 1.0f;
	// ===

	// === Spot Light === //
	float4 spotDirection = normalize(spotLight.position - _input.worldPosition);
	float  coneRatio = saturate(dot(-spotDirection, spotLight.direction));
	float  spotFactor = (coneRatio > spotLight.coneRatio.x) ? 1.0f : 0.0f;
	float  spotRatio = saturate(dot(spotDirection.y, _input.normals)); // spotRatio - To be used for specular lighting
	float  attenuation = 1.0f - saturate((spotLight.coneRatio.x - coneRatio) / (spotLight.coneRatio.x - spotLight.coneRatio.y));
	attenuation *= attenuation;	// Quadratic attenuation
	float4 spotResult = attenuation * spotFactor * spotRatio * spotLight.color * imageColor;
	// ===


	// === Create a Greyscale === //
	float4 greyScale = ambientLight.color * imageColor;
	// ===

	return saturate(greyScale + directionResult + ambientDirection + pointResult + spotResult);
}
// ===

#endif	// DEFAULT_PIXEL_HLSL


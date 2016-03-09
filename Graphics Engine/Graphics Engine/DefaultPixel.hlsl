//////////////////////////////////////////////////////////////////////////
// DefaultPixel.hlsl
// Description:	Nothing special about this shader. Just the default.
// 
// Created By:		Doug Berg
// Last Edited:		3.9.2016
//////////////////////////////////////////////////////////////////////////
#ifndef DEFAULT_PIXEL_HLSL
#define DEFAULT_PIXEL_HLSL



// === Globals === //
struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float4 worldPosition : WORLDPOS;
	float4 normals : NORMALS;
	float2 uv : UV;
};
// ===


texture2D image : register(t0);

SamplerState filter : register(s0);


// === BUFFERS === //
cbuffer LIGHT_BUFFER : register(b0)
{
	// Directional
	float4 diffuseLightDirection : DIRECTION;
	float4 diffuseLightColor : COLOR;

	// Point
	float4 pointLightLocation : LOCATION;
	float4 pointLightColor : PL_COLOR;
}
// ===


float4 main( OUTPUT_VERTEX _input ) : SV_TARGET
{
	float4 imageColor = image.Sample(filter, _input.uv);

	// === Directional Light === //
	float directionRatio = saturate(dot(-diffuseLightDirection, _input.normals));
	float4 directionResult = directionRatio * diffuseLightColor * imageColor;
	directionResult.w = 1;
	// Ambient lighting
	float4 ambientDirection = imageColor * directionResult;


	// === Create a Greyscale === //
	float4 greyScale = { 0.25f, 0.25f, 0.25f, 0.25f };
	greyScale = greyScale * imageColor;
	// ===


	return saturate(greyScale + directionResult + ambientDirection);
}


#endif	// DEFAULT_PIXEL_HLSL


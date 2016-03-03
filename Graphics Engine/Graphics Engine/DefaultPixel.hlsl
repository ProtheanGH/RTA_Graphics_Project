//////////////////////////////////////////////////////////////////////////
// DefaultPixel.hlsl
// Description:	Nothing special about this shader. Just the default.
// 
// Created By:		Doug Berg
// Last Edited:		3.2.2016
//////////////////////////////////////////////////////////////////////////
#ifndef DEFAULT_PIXEL_HLSL
#define DEFAULT_PIXEL_HLSL



// === Globals === //
struct OUTPUT_VERTEX
{
	float4 colorOut : COLOR;
	float4 projectedCoordinate : SV_POSITION;
	float4 worldPosition : WORLDPOS;
	float4 normals : NORMALS;
	float2 uv : UV;
};
// ===



texture2D image : register(t0);

SamplerState filter : register(s0);


// === BUFFERS === //
cbuffer DIFFUSE_LIGHT_BUFFER : register(b0)
{
	float4 diffuseLightDirection : DIRECTION;
	float4 diffuseLightColor : COLOR;
}

cbuffer POINTLIGHT_BUFFER : register(b1)
{
	float4 pointLightPosition : POSITION;
	float4 pointLightColor : COLOR;
}

cbuffer SPOTLIGHT_BUFFER : register(b2)
{
	float4 spotlightPosition : POSITION;
	float4 spotlightDirection : DIRECTION;
	float4 spotlightColor : COLOR;
	float4 spotlightConeRatio : CONE_RATIO;
}

cbuffer SPECULAR_BUFFER : register(b3)
{
	float4 specularPosition : POSITION;
}
// ===


float4 main( OUTPUT_VERTEX _input ) : SV_TARGET
{
	float4 imageColor = image.Sample(filter, _input.uv);


	// === Directional Light === //
	float directionRatio = saturate(dot(-diffuseLightDirection, _input.normals));
	float4 directionResult = directionRatio * diffuseLightColor * imageColor;
	directionResult.w = 1;


	// === Point Light === //
	float4 pointDirection = normalize(pointLightPosition - _input.worldPosition);
	float pointRatio = saturate(dot(pointDirection, _input.normals));
	float4 pointResult = pointRatio * pointLightColor * imageColor;
	pointResult.w = 1;


	// === Spot Light === //
	float4 spotlightConeDirection = normalize(spotlightPosition - _input.worldPosition);
	float  spotRatio = saturate(dot(spotlightDirection, -spotlightConeDirection));
	float  spotFactor = (spotRatio > spotlightConeRatio.y) ? 1.0f : 0.0f;
	float  spotlightRatio = saturate(dot(spotlightConeDirection.y, _input.normals));
	float attenuation = 1.0f - saturate((spotlightConeRatio.x - spotRatio) / (spotlightConeRatio.x - spotlightConeRatio.y));
	attenuation *= attenuation;
	float4 spotlightResult = spotFactor * spotRatio * spotlightColor * imageColor * attenuation;
	// ===


	// === Specular Light Using Spot Light === //
	float4 toCamDirection = normalize(specularPosition - _input.worldPosition);
	float4 reflection = normalize(reflect(-spotlightConeDirection, _input.normals));
	float  intensity = pow(saturate(dot(reflection, toCamDirection)), 128);
	float4 spotLightSpecularResult = saturate(intensity * spotFactor * spotlightRatio * attenuation * float4(1.0f, 1.0f, 1.0f, 1.0f));

	// === Specular Light Using Point Light === //
	toCamDirection = normalize(specularPosition - _input.worldPosition);
	reflection = normalize(reflect(-pointDirection, _input.normals));
	intensity = pow(saturate(dot(reflection, toCamDirection)), 128);
	float4 pointLightSpecularResult = saturate(intensity * float4(1.0f, 1.0f, 1.0f, 1.0f));

	// === Create a Greyscale === //
	float4 greyScale = { 0.25f, 0.25f, 0.25f, 0.25f };
	greyScale = greyScale * imageColor;
	// ===


	return saturate(directionResult + pointResult + spotlightResult + greyScale + spotLightSpecularResult + pointLightSpecularResult);
}


#endif	// DEFAULT_PIXEL_HLSL


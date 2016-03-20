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
	float radius;

	float padding;
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
	float4 ambientColor, directionalColor;

	// === Sample out the two textures
	textureColor = diffuseTexture.Sample(filter, _input.texCoords);
	normalMap = normalTexture.Sample(filter, _input.texCoords);

	// === Convert from (0 - +1) range to (-1 - +1) range
	normalMap = (normalMap * 2.0f) - 1.0f;

	// === Calculate the Normal
	float3x3 TBNMatrix = float3x3((float3)_input.tangent, (float3)_input.binormal, (float3)_input.normal);
	normal = mul((float3)normalMap, TBNMatrix);
	normal = normalize(normal);

	// === Ambient Lighting
	ambientColor = ambientLight.color * textureColor;

	// === Directional Lighting
	lightIntensity = saturate(dot(normal, (float)-directionalLight.direction));
	directionalColor = saturate(directionalLight.color * lightIntensity);
	directionalColor = directionalColor * textureColor;
	directionalColor.w = 1.0;

	// === Point Light === //
	float4 pointDirection = normalize(pointLight.position - _input.worldPosition);
	float  pointRatio = saturate(dot(pointDirection, _input.normal));
	float  pointAtten = 1.0f - saturate(length(pointLight.position - _input.worldPosition) / pointLight.radius);
	pointAtten *= pointAtten;
	float4 pointResult = pointAtten * pointRatio * pointLight.color * textureColor;
	pointResult.w = 1.0f;
	// ===

	// === Spot Light === //
	float4 spotDirection = normalize(spotLight.position - _input.worldPosition);
	float  coneRatio = saturate(dot(-spotDirection, spotLight.direction));
	float  spotFactor = (coneRatio > spotLight.coneRatio.x) ? 1.0f : 0.0f;
	float  spotRatio = saturate(dot(spotDirection.y, _input.normal)); // spotRatio - To be used for specular lighting
	float  attenuation = 1.0f - saturate((spotLight.coneRatio.x - coneRatio) / (spotLight.coneRatio.x - spotLight.coneRatio.y));
	attenuation *= attenuation;	// Quadratic attenuation
	float4 spotResult = attenuation * spotFactor * spotRatio * spotLight.color * textureColor;
	// ===


	return ambientColor + directionalColor + pointResult + spotResult;
}
// ===

#endif
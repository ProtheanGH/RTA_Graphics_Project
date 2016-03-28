#ifndef AnimationVertex_hlsl
#define AnimationVertex_hlsl

#pragma pack_matrix(row_major)

struct Bone
{
	float4 position : POSITION;
	float4 normal : NORMALS;
	float4 texCoords : UV;					// Changed from Float2 to Float4
	float4 tangent : TANGENT;
	float4 binormal : BINORMAL;
	float4 weight : WEIGHT;
	int4 indice : INDICE;
	int4 cp_Padding : CP_PADDING;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 worldPosition : WORLDPOS;
	float4 texCoords : UV;					// Changed from Float2 to Float4
	float4 normal : NORMALS;
	float4 tangent : TANGENT;
	float4 binormal : BINORMAL;
};

cbuffer Object : register(b0)
{
	float4x4 worldMatrix;
	float4x4 bonesMatrices[50];
	int boneAmount;

	float3 padding;
};

cbuffer Scene : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projMatrix;
};


PixelInput main(Bone _input)
{
	PixelInput output;
/*
	// === Make sure the position is in the correct format
	_input.position.w = 1.0f;

	// === Convert the position into the correct space
	output.position = mul(_input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projMatrix);

	output.worldPosition = mul(_input.position, worldMatrix);

	output.texCoords.xy = _input.texCoords.xy;
	output.texCoords.z = output.texCoords.w = 0;

	// === Convert the Normal, Tangent, and Binormal into world space
	output.normal = mul(_input.normal, worldMatrix);
	output.normal = normalize(output.normal);
	output.tangent = mul(_input.tangent, worldMatrix);
	output.tangent = normalize(output.tangent);
	output.binormal = mul(_input.binormal, worldMatrix);
	output.binormal = normalize(output.binormal);
*/

	// Animations
	float4x4 animationMat;
 	animationMat =  bonesMatrices[_input.indice.x] * _input.weight.x; // _input.weight.y; 
 	animationMat += bonesMatrices[_input.indice.y] * _input.weight.y; // _input.weight.y; 
 	animationMat += bonesMatrices[_input.indice.z] * _input.weight.z; // _input.weight.y;
 	animationMat += bonesMatrices[_input.indice.w] * _input.weight.w; // _input.weight.y; 
//	animationMat = bonesMatrices[0];

	output.position = mul(_input.position, animationMat);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projMatrix);

//	output.normal   = mul( _input.normal, animationMat );

	// World Position
	output.worldPosition = mul(_input.position, worldMatrix);

	// Texture
	output.texCoords = _input.texCoords;

	// Normal Mapping
	output.normal = mul(_input.normal, animationMat);
	output.normal = normalize(output.normal);
	output.tangent = mul(_input.tangent, animationMat);
	output.tangent = normalize(output.tangent);
	output.binormal = mul(_input.binormal, animationMat);
	output.binormal = normalize(output.binormal);

	return output;
}
#endif

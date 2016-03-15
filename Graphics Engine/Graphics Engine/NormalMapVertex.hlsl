#ifndef NormalMapVertex_HLSL
#define NormalMapVertex_HLSL

#pragma pack_matrix(row_major)

// === Shader Input Structures === //
struct VertexInput 
{
	float4 position : POSITION;
	float4 normal : NORMALS;
	float2 texCoords : UV;
	float4 tangent : TANGENT;
	float4 binormal : BINORMAL;
};

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
cbuffer Object : register(b0)
{
	float4x4 worldMatrix;
}

cbuffer Scene : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projection;
}
// ======================== //

// === Main Function
PixelInput main( VertexInput _input )
{
	PixelInput output;

	// === Make sure the position is in the correct format
	_input.position.w = 1.0f;

	// === Convert the position into the correct space
	output.position = mul(_input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projection);

	output.worldPosition = _input.position;

	output.texCoords = _input.texCoords;

	// === Convert the Normal, Tangent, and Binormal into world space
	output.normal = mul(_input.normal, worldMatrix);
	output.normal = normalize(output.normal);
	output.tangent = mul(_input.tangent, worldMatrix);
	output.tangent = normalize(output.tangent);
	output.binormal = mul(_input.binormal, worldMatrix);
	output.binormal = normalize(output.binormal);

	return output;
}
// ===

#endif
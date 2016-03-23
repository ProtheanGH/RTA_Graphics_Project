//////////////////////////////////////////////////////////////////////////
// DefaultVertex.hlsl
// Description:	Nothing special about this shader. Just the default.
// 
// Created By:		Doug Berg
// Last Edited:		3.2.2016
//////////////////////////////////////////////////////////////////////////
#ifndef DEFAULT_VERTEX_HLSL
#define DEFAULT_VERTEX_HLSL

#pragma pack_matrix(row_major)


// === Globals === //
struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
	float4 normals : NORMALS;
	float4 uv : UV;					// Changed from Float2 to Float4
};

struct OUTPUT_VERTEX
{
	float4 projectedCoordinate : SV_POSITION;
	float4 worldPosition : WORLDPOS;
	float4 normals : NORMALS;
	float4 uv : UV;					// Changed from Float2 to Float4
};

cbuffer Object : register(b0)
{
	float4x4 worldMatrix;
}

cbuffer Scene : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projection;
}
// ===



OUTPUT_VERTEX main( INPUT_VERTEX _input )
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;

	// Projected Position
	float4 pos;
	_input.coordinate.w = 1;
	pos = _input.coordinate;
	pos = mul(pos, worldMatrix);
	output.worldPosition = mul(_input.coordinate, worldMatrix);
	pos = mul(pos, viewMatrix);
	pos = mul(pos, projection);
	output.projectedCoordinate = pos;

	// Normals
	output.normals.w = 0;
	output.normals = mul(_input.normals, worldMatrix);

	// UV
	output.uv = _input.uv;
	
	return output;
}

#endif // DEFAULT_VERTEX_HLSL


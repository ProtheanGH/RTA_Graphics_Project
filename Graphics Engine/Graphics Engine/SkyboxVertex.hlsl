#pragma pack_matrix(row_major)


struct INPUT_VERTEX
{
	float4 coordinate : POSITION;
	float4 normals : NORMALS;
	float2 uv : UV;
};

struct OUT_VERTEX
{
	float4 worldPosition : W_POSITION;
	float4 projectedPosition : SV_POSITION;
	float3 coord : TEXCOORD;
	float  padding : PAD;
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

OUT_VERTEX main( INPUT_VERTEX _input )
{
	OUT_VERTEX output = (OUT_VERTEX)0;

	float4 pos;

	pos = _input.coordinate;
	pos = mul(pos, worldMatrix);
	pos = mul(pos, viewMatrix);
	pos = mul(pos, projection);
	output.projectedPosition = pos;
	_input.coordinate.w = 1;

	// UV
	output.coord = _input.coordinate.xyz;

	return output;
}
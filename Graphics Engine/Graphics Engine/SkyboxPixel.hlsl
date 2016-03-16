



struct OUT_VERTEX
{
	float4 worldPosition : W_POSITION;
	float4 projectedPosition : SV_POSITION;
	float3 coord : TEXCOORD;
	float  padding : PAD;
};


TextureCube image : register(t0);


SamplerState filter : register(s0);


float4 main(OUT_VERTEX _input) : SV_TARGET
{
	return image.Sample(filter, _input.coord.xyz);
}




struct OUT_VERTEX
{
	float4 position : SV_POSITION;
	float3 coord : TEXCOORD;
};


TextureCube image : register(t0);


SamplerState filter : register(s0);


float4 main(OUT_VERTEX _input) : SV_TARGET
{
	return image.Sample(filter, _input.coord.xyz);
}
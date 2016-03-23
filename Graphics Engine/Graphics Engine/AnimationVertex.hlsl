struct Bone
{
	float4 position : POSITION;
	float4 normal : NORMALS;
	float2 texCoords : UV;
	float4 tangent : TANGENT;
	float4 binormal : BINORMAL;
	float4 weight : WEIGHT;
	int4 indice : INDICE;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 worldPosition : WORLDPOS;
	float2 texCoords : UV;
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
	PixelInput output = (PixelInput)0;

	// Animations
	float4x4 animationMat;
	animationMat  = bonesMatrices[ _input.indice.x ] * _input.weight.x;
	animationMat += bonesMatrices[ _input.indice.y ] * _input.weight.y;
	animationMat += bonesMatrices[ _input.indice.z ] * _input.weight.z;
	animationMat += bonesMatrices[ _input.indice.w ] * _input.weight.w;
	
	output.position = mul(_input.position, animationMat); 
	output.position = mul(_input.position, viewMatrix);
	output.position = mul(output.position, projMatrix);
	output.normal   = mul( _input.normal, animationMat );

	// Position
	 //_input.position.w = 1.0f;
	 //output.position = mul(_input.position, worldMatrix);
	 //output.position = mul(_input.position, viewMatrix);
	 //output.position = mul(_input.position, projMatrix);

	// World Position
	output.worldPosition = mul(_input.position, worldMatrix);

	// Texture
	output.texCoords = _input.texCoords;

	// Normal Mapping
	output.normal = mul(_input.normal, worldMatrix);
	output.normal = normalize(output.normal);
	output.tangent = mul(_input.tangent, worldMatrix);
	output.tangent = normalize(output.tangent);
	output.binormal = mul(_input.binormal, worldMatrix);
	output.binormal = normalize(output.binormal);

	return output;
}
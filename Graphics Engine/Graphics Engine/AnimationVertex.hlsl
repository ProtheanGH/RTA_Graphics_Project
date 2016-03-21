struct Bone
{
	float4 position : POSITION;
	float4 normal : NORMALS;
	float2 texCoords : UV;
	float4 tangent : TANGENT;
	float4 binormal : BINORMAL;
	float4 weight : WEIGHT;
	int4 indice : INDICE;
	int size : SIZE;
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
};

cbuffer Scene : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projMatrix;
};

cbuffer Bone : register(b2)
{
	int arr[64];
};


PixelInput main(Bone _input)
{
	PixelInput output = (PixelInput)0;

	// Position
	_input.position.w = 1.0f;
	output.position = mul(_input.position, worldMatrix);
	output.position = mul(_input.position, viewMatrix);
	output.position = mul(_input.position, projMatrix);

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
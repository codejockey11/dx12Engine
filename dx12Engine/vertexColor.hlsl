struct SPixel
{
	float4 position	: SV_POSITION;
	float4 color	: COLOR;
};

cbuffer float4x4s : register(b0)
{
	row_major float4x4 worldMatrix;
	row_major float4x4 viewMatrix;
	row_major float4x4 projectionMatrix;
	row_major float4x4 fill[13];
};

SPixel VSMain(float3 position : POSITION, float4 color : COLOR)
{
	SPixel result;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	float4 pos = float4(position.xyz, 1.0f);

	result.position = mul(pos, worldMatrix);
	result.position = mul(result.position, viewMatrix);
	result.position = mul(result.position, projectionMatrix);

	result.color = color;

	return result;
}
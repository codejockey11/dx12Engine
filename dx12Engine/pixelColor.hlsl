struct SPixel
{
	float4 position	: SV_POSITION;
	float4 color	: COLOR;
};

// 256 / 16 = 16
cbuffer float4x4s : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4x4 fill[13];
};

// 256 / 4 = 64 floats
cbuffer floats : register(b1)
{
	float fv0;
	float fv1;
	float fv2;
	float fv3;
	float fv4;
	float fv5;
	float fv6;
	float fill1[57];
};

// 256 / 4 = 64 floats
cbuffer ints : register(b2)
{
	int iv0;
	int iv1;
	int iv2;
	int iv3;
	int iv4;
	int fill2[59];
};


float4 PSMain(SPixel vs) : SV_Target0
{
	return vs.color;
}
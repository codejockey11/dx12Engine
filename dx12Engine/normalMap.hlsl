//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4x4 world					: WORLD;
float4x4 wvp	   				: WORLDVIEWPROJ;

float3 lightDir					: register(c0);
float4 lightDiffuse				: register(c1);
float4 lightAmbient				: register(c2);
float4 materialDiffuseColor		: register(c3);

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
texture texture00 : register(t0);
sampler2D sampler00 : register(s0) = sampler_state
{
	Texture = (texture00);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

texture texture01 : register(t1);
sampler2D sampler01 : register(s1) = sampler_state
{
	Texture = (texture01);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

texture texture02 : register(t2);
sampler2D sampler02 : register(s2) = sampler_state
{
	Texture = (texture02);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

texture texture03 : register(t3);
sampler2D sampler03 : register(s3) = sampler_state
{
	Texture = (texture03);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

texture texture04 : register(t4);
sampler2D sampler04 : register(s4) = sampler_state
{
	Texture = (texture04);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_STRUCT
{
	float4 pos		: POSITION;   // vertex position
	float4 diffuse  : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
	float2 uv		: TEXCOORD0;  // vertex texture coords
};


//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_STRUCT VSMain(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0)
{
	VS_STRUCT vertex;
	float3 normalWorldSpace;

	// Transform the position from object space to homogeneous projection space
	vertex.pos = mul(pos, wvp);

	// Transform the normal from object space to world space
	normalWorldSpace = normalize(mul(normal, (float3x3)world)); // normal (world space)

																// Compute simple directional lighting equation
	float3 totalLightDiffuse = float3(0, 0, 0);
	totalLightDiffuse += lightDiffuse * max(0, dot(normalWorldSpace, lightDir));

	vertex.diffuse.rgb = materialDiffuseColor * totalLightDiffuse * lightAmbient;
	vertex.diffuse.a = 1.0f;

	vertex.uv = uv;

	return vertex;

}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_STRUCT
{
	float4 rgb : COLOR0;  // Pixel color
};


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_STRUCT PSMain(VS_STRUCT vs)
{
	PS_STRUCT pixel;

	pixel.rgb = tex2D(sampler03, vs.uv) *  vs.diffuse * dot(lightDir, tex2D(sampler04, vs.uv));
	pixel.rgb.a = 1.0f;

	return pixel;

}


//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique NormalMap
{
	pass P0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}

}

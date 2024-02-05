//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4x4 world		: WORLD;
float4x4 view		: VIEW;
float4x4 proj		: PROJ;

float4x4 wvp		: WORLDVIEWPROJ;

float  fogStart		: register(c0);
float  fogEnd		: register(c1);
float  fogDensity	: register(c2);
float  minFog		: register(c3);
float  maxFog		: register(c4);
float3 fogColor		: register(c5);

//--------------------------------------------------------------------------------------
// Lighting
//--------------------------------------------------------------------------------------
float3 lightDir					: register(c6);
float4 lightDiffuse				: register(c7);
float4 lightAmbient				: register(c8);
float4 materialDiffuseColor		: register(c9);

float4 light1Position			: register(c10);
float  light1Radius				: register(c11);
float4 light1Color				: register(c12);
float  light1Linear				: register(c13);
float  light1Quadratic			: register(c14);
float  light1Intensity			: register(c15);

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
Texture2D texture00 : register(t0);
sampler2D sampler00 : register(s0) = sampler_state
{
	Texture = (texture00);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

Texture2D texture01 : register(t1);
sampler2D sampler01 : register(s1) = sampler_state
{
	Texture = (texture01);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

Texture2D texture02 : register(t2);
sampler2D sampler02 : register(s2) = sampler_state
{
	Texture = (texture02);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

Texture2D texture03 : register(t3);
sampler2D sampler03 : register(s3) = sampler_state
{
	Texture = (texture03);
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

Texture2D texture04 : register(t4);
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
	float  depth	: DEPTH;
	float4 vPos		: TEXCOORD1;
	float3 normal	: TEXCOORD2;
};

//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_STRUCT VSMain(float4 vPos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD0)
{
	VS_STRUCT vertex;

	// Transform the position from object space to homogeneous projection space
	vertex.pos = mul(vPos, wvp);

	vertex.vPos = vPos;

	vertex.normal = normal;

	vertex.depth = vertex.pos.z;

	vertex.uv = uv;

	// Transform the normal from object space to world space
	float3 normalWorldSpace = normalize(mul(normal, (float3x3)world));

	// Compute simple directional lighting equation

	float4 totalLightDiffuse = float4(0,0,0,1);
	totalLightDiffuse += lightDiffuse * max(0, abs(dot(lightDir, normal)));

	vertex.diffuse = (materialDiffuseColor * totalLightDiffuse * lightAmbient);
	vertex.diffuse.a = 1.0f;

	return vertex;
}

//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_STRUCT
{
	float4 rgb : COLOR0;
};

PS_STRUCT PSMain(VS_STRUCT vs)
{
	PS_STRUCT pixel;

	pixel.rgb = tex2D(sampler02, vs.uv);

	float4 maskMap = tex2D(sampler01, vs.uv);
	
	if (maskMap.r > 0.0)
	{
		float b1 = 1.0 - maskMap.r;
		float b2 = 1.0 - b1;
		
		float4 ts = tex2D(sampler00, vs.uv);
		
		pixel.rgb = (pixel.rgb * b1) + (ts * b2);
	}

	if (fogDensity > 0.0)
	{
		float fog = (vs.depth - fogStart) / (fogEnd - fogStart);
		fog *= fogDensity;
		fog = clamp(fog, minFog, maxFog);
		pixel.rgb = lerp(pixel.rgb, float4(fogColor, 1.0), fog);
	}


	float4 lightFinal = float4(0,0,0,1);

	if(light1Radius > 0.0)
	{
		float4 pointVector = light1Position - vs.vPos;
		float dist = length(pointVector);

		// if for spotlight
		if(dist <= light1Radius)
		{
			// pointlight attenuation
			// a = intensity / (1.0 + (linear * dist) + (quadratic * dist^2));
			float attenuation = light1Intensity / (1.0 + (light1Linear * dist) + (light1Quadratic * (dist * dist)));

			float NdL = max(1.0, dot(vs.normal, normalize(pointVector)));

			float4 light1Final = float4(0,0,0,1);

			light1Final.r = NdL * light1Color.rgb.r * attenuation;
			light1Final.g = NdL * light1Color.rgb.g * attenuation;
			light1Final.b = NdL * light1Color.rgb.b * attenuation;

			lightFinal += light1Final;
		}
	}

	pixel.rgb *= (vs.diffuse + lightFinal);

	// =================================================================================
	// Return Color

	pixel.rgb.a = 1.0;

	return pixel;
}

technique TerrainBlendLighting
{
	pass P0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}

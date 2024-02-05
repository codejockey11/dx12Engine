struct SPixel
{
	float4 position	: SV_POSITION;
	float3 normal	: NORMAL;
	float2 uv		: TEXCOORD;
	float3 vsPos	: POSITION;
};

// 256 / 16 = 16 float4x4s per buffer
cbuffer float4x4s : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4x4 fill[13];
};

// 256 / 4 = 64 floats per buffer
cbuffer floats : register(b1)
{
	float fv0;
	float fv1;
	float fv2;
	float fv3;
	float fv4;
	float fv5;
	float fv6;
	float fv7;
	float fill1[56];
};

// 256 / 4 = 64 ints per buffer
cbuffer ints : register(b2)
{
	int iv0;
	int iv1;
	int iv2;
	int iv3;
	int iv4;
	float fill2[59];
};

SamplerState anisotropic : register(s0);
SamplerState wrap		 : register(s1);

Texture2D	textureKd : register(t0);
Texture2D	textureKb : register(t1);
Texture2D	textureKs : register(t2);
Texture2D	textureKa : register(t3);
Texture2D	textureKr : register(t4);

float4 PSMain(SPixel vs) : SV_Target0
{
	float4 p = { 0, 0, 0, 1 };

	float pa = p.a;

	// diffuse texture
	if (iv0 > 0)
	{
		p = textureKd.Sample(anisotropic, vs.uv);

		pa = p.a;

		//p *= 0.75;
	}

	// normal map texture
	if (iv1 > 0)
	{
		float3 nmap;
		
		nmap.r = textureKb.Sample(anisotropic, vs.uv).r;
		nmap.g = textureKb.Sample(anisotropic, vs.uv).g;
		nmap.b = textureKb.Sample(anisotropic, vs.uv).b;

		float3 vsp = { 0, 1, 0 };

		float ndl = dot(nmap, normalize(vsp));

		clamp(ndl, 0.25, 1.0);

		//p *= ndl;
	}

	// specular texture
	if (iv2 > 0)
	{
		//p *= textureKs.Sample(anisotropic, vs.uv);
	}

	// alpha mask texture
	if (iv3 > 0)
	{
		pa = textureKa.Sample(anisotropic, vs.uv).r;
	}

	// reflection texture
	if (iv4 > 0)
	{
		//p = textureKr.Sample(anisotropic, vs.uv);
	}

	p.a = pa;

	return p;


	if (fv3 > 0.0)
	{
		float3 vsp = { (fv0 - vs.vsPos.x), (fv1 - vs.vsPos.y), (fv2 - vs.vsPos.z) };

		float l = length(vsp);

		// if for spotlight
		if (l <= fv7)
		{
			float a = fv6 / l;

			p.r *= fv3 * a;
			p.g *= fv4 * a;
			p.b *= fv5 * a;
		}
	}
}
#pragma once

#include "standard.h"

#include "CString.h"
#include "CTexture.h"

class CMaterial
{
public:

	enum
	{
		MATERIAL_COUNT = 8,
		MATERIAL_LENGTH = 32
	};

	CString* m_name;

	XMFLOAT3 m_ambient;           // Ka
	XMFLOAT3 m_diffuse;           // Kd
	XMFLOAT3 m_specular;          // Ks
	XMFLOAT3 m_emissive;          // Ke

	float m_specularExponent;  // Ns

	float m_opacity;           // d or inverted: Tr = 1 - d

	UINT m_illum;

	float m_opticalDensity;    // Specifies the optical density for the surface.This is also known as index of refraction.

	CTexture* m_map_Ka;	// the ambient texture map
	CTexture* m_map_Kd;	// the diffuse texture map(most of the time, it will be the same as the ambient texture map
	CTexture* m_map_Ks;   // specular color texture map
	CTexture* m_map_Ns;   // specular highlight component
	CTexture* m_map_d;    // the alpha texture map
	CTexture* m_map_Kb;   // some implementations use 'map_bump' instead of 'bump' below

	CTexture* m_refl;     // spherical reflection map


	CMaterial();

	CMaterial(const char* name);

	~CMaterial();


	void Copy(CMaterial* material);
};
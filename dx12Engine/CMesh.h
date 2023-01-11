#pragma once

#include "standard.h"

#include "CMaterial.h"
#include "CString.h"
#include "CTexture.h"

class CMesh
{
public:

	CMaterial* m_material;

	BYTE* m_vertices;

	UINT m_vertexCount;

	CString* m_map_Ka;
	CString* m_map_Kd;
	CString* m_map_Ks;
	CString* m_map_Ns;
	CString* m_map_d;
	CString* m_map_Kb;
	CString* m_refl;

	
	CMesh();

	~CMesh();
};
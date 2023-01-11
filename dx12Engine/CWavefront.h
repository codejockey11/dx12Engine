#pragma once

#include "standard.h"

#include "CLinkList.h"
#include "CMesh.h"
#include "CString.h"
#include "CVertex.h"

// Model texture needs to be a top left tga

class CWavefront
{
public:

	enum
	{
		VIEW_COUNT = 65536,
		LINE_LENGTH = 132,
	};

	CString* m_name;

	CString* m_materialname;

	CLinkList<CMesh>* m_meshs;

	bool m_isInitialized;

	CVertex* m_vertex;

	CVertex* m_normal;

	CVertex2* m_uv;

	char m_materialName[CMaterial::MATERIAL_COUNT][CMaterial::MATERIAL_LENGTH];

	UINT m_vi[8];

	UINT m_ni[8];

	UINT m_uvi[8];

	CVertexNT* m_vnt[CMaterial::MATERIAL_COUNT];

	CVertexNT* m_vntl[CMaterial::MATERIAL_COUNT];

	int m_vntcount;

	int m_icount[CMaterial::MATERIAL_COUNT];

	CMesh* m_mesh[CMaterial::MATERIAL_COUNT];


	CWavefront();

	CWavefront(const char* filename, const char* materialFilename);

	~CWavefront();


	void WriteVertices(FILE* file, CVertex* p);

	void LoadMeshList();

private:

	int m_inbr;
};
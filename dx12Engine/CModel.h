#pragma once

#include "standard.h"

#include "CLinkList.h"
#include "CMesh.h"
#include "CString.h"
#include "CVertex.h"

class CModel
{
public:

	enum
	{
		MATERIAL_LENGTH = 256
	};

	short m_materialCount;

	CLinkList<CMesh>* m_meshs;

	CMesh* m_mesh;

	bool m_isInitialized;

	CString* m_name;


	CModel();

	CModel(const char* filename);

	~CModel();
};
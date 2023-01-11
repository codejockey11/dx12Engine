#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CString.h"
#include "CVertex.h"
#include "CVertexBuffer.h"

// Model texture needs to be an uncompressed, 32 bit, top left tga

class CObjModel
{
public:

	enum
	{
		MAX_COUNT = 65536,
		MAX_LINE_LENGTH = 128,
		MAX_MATERIAL_COUNT = 16,
		MAX_MATERIAL_LENGTH = 32
	};

	CString* filename;

	CVertex vertex[CObjModel::MAX_COUNT];
	
	CVertex normal[CObjModel::MAX_COUNT];
	
	CVertex2 uv[CObjModel::MAX_COUNT];

	CHAR materialName[CObjModel::MAX_MATERIAL_COUNT][CObjModel::MAX_MATERIAL_LENGTH];

	UINT vi[8];

	UINT ni[8];

	UINT uvi[8];

	CVertexNT vnt[CObjModel::MAX_COUNT];

	UINT vntcount;

	CVertexBuffer* vertexBuffer[CObjModel::MAX_MATERIAL_COUNT];

	UINT icount[CObjModel::MAX_MATERIAL_COUNT];

	CObjModel();

	CObjModel(CGlobalObjects* g, const char* filename);

	~CObjModel();

	VOID Draw();

private:

	CGlobalObjects* globalObjects;

	FILE* file;

	errno_t err;

	size_t br;

	CHAR line[CObjModel::MAX_LINE_LENGTH];

	UINT vindex;
	UINT nindex;
	UINT uvindex;

	UINT inbr;

	INT maxIndex;
};
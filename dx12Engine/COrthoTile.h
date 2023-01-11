#pragma once

#include "standard.h"

#include "CBoundBox.h"
#include "CGlobalObjects.h"
#include "CIndexBuffer.h"
#include "CVertex.h"
#include "CVertexBuffer.h"

class COrthoTile
{
public:

	enum
	{
		OT_NONE = 0,

		OT_CLIP,

		OT_REDSTART,
		OT_BLUESTART,

		OT_LOCK,
		OT_KEY
	};

	BYTE m_type;

	CVertex2 m_position;
	CVertex2 m_size;
	CVertex2 m_sheetSize;
	CVertex2 m_cellSize;

	CBoundBox* m_box;

	CVertexT m_vertices[4];

	CVertexBuffer* m_vertexBuffer;

	CIndexBuffer* m_indexBuffer;

	
	COrthoTile();

	COrthoTile(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList, BYTE type, CVertex2* ss, CVertex2* wh, CVertex2* coord, CVertex2* s, CVertex2* p);

	~COrthoTile();

	
	void UpdatePosition();

	void Draw();

private:

	CGlobalObjects* m_globalObjects;

	ComPtr<ID3D12GraphicsCommandList> m_commandList;
};
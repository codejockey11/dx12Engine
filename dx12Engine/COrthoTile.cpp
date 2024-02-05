#include "COrthoTile.h"

COrthoTile::COrthoTile()
{
	memset(this, 0x00, sizeof(COrthoTile));
}

COrthoTile::COrthoTile(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList, BYTE type, CVertex2* sheetSize, CVertex2* cellSize, CVertex2* uvCoordinate, CVertex2* size, CVertex2* position)
{
	memset(this, 0x00, sizeof(COrthoTile));

	m_globalObjects = globalObjects;

	m_commandList = commandList;

	m_type = type;

	m_sheetSize = *sheetSize;
	m_cellSize = *cellSize;
	m_size = *size;
	m_position = *position;

	float uvLeft;
	float uvTop;
	float uvRight;
	float uvBottom;

	if (uvCoordinate->p.x == 0)
	{
		uvLeft = (1.0f / m_sheetSize.p.x) * 0.0f;
		uvRight = (1.0f / m_sheetSize.p.x) * m_cellSize.p.x;
	}
	else
	{
		uvLeft = (1.0f / m_sheetSize.p.x) * (m_cellSize.p.x * uvCoordinate->p.x);
		uvRight = uvLeft + (1.0f / m_sheetSize.p.x) * m_cellSize.p.x;
	}

	if (uvCoordinate->p.y == 0)
	{
		uvTop = (1.0f / m_sheetSize.p.y) * 0.0f;
		uvBottom = (1.0f / m_sheetSize.p.y) * m_cellSize.p.y;
	}
	else
	{
		uvTop = (1.0f / m_sheetSize.p.y) * (m_cellSize.p.y * uvCoordinate->p.y);
		uvBottom = uvTop + (1.0f / m_sheetSize.p.y) * m_cellSize.p.y;
	}

	// vertex uv draw order
	// 0---1
	// | \ |
	// 3---2

	m_vertices[0].p.x = 0.0f;
	m_vertices[0].p.y = 0.0f;

	m_vertices[0].uv.x = uvLeft;
	m_vertices[0].uv.y = uvTop;

	m_vertices[1].p.x = m_size.p.x;
	m_vertices[1].p.y = 0.0f;

	m_vertices[1].uv.x = uvRight;
	m_vertices[1].uv.y = uvTop;

	m_vertices[2].p.x = m_size.p.x;
	m_vertices[2].p.y = m_size.p.y;

	m_vertices[2].uv.x = uvRight;
	m_vertices[2].uv.y = uvBottom;

	m_vertices[3].p.x = 0.0f;
	m_vertices[3].p.y = m_size.p.y;

	m_vertices[3].uv.x = uvLeft;
	m_vertices[3].uv.y = uvBottom;

	m_vertexBuffer = new CVertexBuffer(globalObjects, commandList, CVertex::VT_VERTEXT, 4, (void*)m_vertices);

	DWORD index[6];

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;

	index[3] = 0;
	index[4] = 2;
	index[5] = 3;

	m_indexBuffer = new CIndexBuffer(globalObjects, commandList, 6, index);

	CVertex v1(m_vertices[0].p.x, m_vertices[0].p.y, 0.0f);
	CVertex v2(m_vertices[2].p.x, m_vertices[2].p.y, 0.0f);

	m_box = new CBoundBox(&v1, &v2);

	COrthoTile::UpdatePosition();
}

void COrthoTile::UpdatePosition()
{
	m_vertices[0].p.x = m_position.p.x;
	m_vertices[1].p.x = m_position.p.x + m_size.p.x;
	m_vertices[2].p.x = m_position.p.x + m_size.p.x;
	m_vertices[3].p.x = m_position.p.x;

	m_vertices[0].p.y = m_position.p.y;
	m_vertices[1].p.y = m_position.p.y;
	m_vertices[2].p.y = m_position.p.y + m_size.p.y;
	m_vertices[3].p.y = m_position.p.y + m_size.p.y;

	if ((m_vertices[1].p.x < 0.0f) || (m_vertices[3].p.y < 0.0f) || (m_vertices[0].p.x > m_globalObjects->m_window->m_width) || (m_vertices[1].p.y > m_globalObjects->m_window->m_height))
	{
		return;
	}

	CVertex v1(m_vertices[0].p.x, m_vertices[0].p.y, 0.0f);
	CVertex v2(m_vertices[2].p.x, m_vertices[2].p.y, 0.0f);

	m_box->Update(&v1, &v2);

	m_vertexBuffer->LoadBuffer((void*)m_vertices);
}

COrthoTile::~COrthoTile()
{
	delete m_box;

	delete m_indexBuffer;

	delete m_vertexBuffer;
}

void COrthoTile::Draw()
{
	m_indexBuffer->Draw();

	m_vertexBuffer->DrawIndexed(m_indexBuffer->m_count);
}
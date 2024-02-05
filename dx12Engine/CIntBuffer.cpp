#include "CIntBuffer.h"

/*
*/
CIntBuffer::CIntBuffer()
{
	memset(this, 0x00, sizeof(CIntBuffer));
}

/*
*/
CIntBuffer::CIntBuffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	memset(this, 0x00, sizeof(CIntBuffer));

	m_globalObjects = globalObjects;

	m_size = CIntBuffer::E_VIEW_COUNT * sizeof(int);

	m_count = CIntBuffer::E_VIEW_COUNT;

	m_stride = sizeof(int);

	m_commandList = commandList;

	m_constantBuffer = new CConstantBuffer(m_globalObjects, m_commandList, m_size);
}

/*
*/
CIntBuffer::~CIntBuffer()
{
	delete m_constantBuffer;
}

/*
*/
void CIntBuffer::UpdateConstantBuffer()
{
	m_constantBuffer->UpdateBuffer((void*)m_values);
}
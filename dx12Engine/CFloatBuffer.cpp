#include "CFloatBuffer.h"

/*
*/
CFloatBuffer::CFloatBuffer()
{
	memset(this, 0x00, sizeof(CFloatBuffer));
}

/*
*/
CFloatBuffer::CFloatBuffer(CGlobalObjects* g, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	memset(this, 0x00, sizeof(CFloatBuffer));

	m_globalObjects = g;

	m_size = CFloatBuffer::E_VIEW_COUNT * sizeof(float);

	m_count = CFloatBuffer::E_VIEW_COUNT;

	m_stride = sizeof(float);

	m_commandList = commandList;

	m_constantBuffer = new CConstantBuffer(m_globalObjects, m_commandList, m_size);
}

/*
*/
CFloatBuffer::~CFloatBuffer()
{
	delete m_constantBuffer;
}

/*
*/
void CFloatBuffer::UpdateConstantBuffer()
{
	m_constantBuffer->UpdateBuffer((void*)m_values);
}
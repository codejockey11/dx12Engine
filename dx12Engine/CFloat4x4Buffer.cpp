#include "CFloat4x4Buffer.h"

/*
*/
CFloat4x4Buffer::CFloat4x4Buffer()
{
	memset(this, 0x00, sizeof(CFloat4x4Buffer));
}

/*
*/
CFloat4x4Buffer::CFloat4x4Buffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList)
{
	memset(this, 0x00, sizeof(CFloat4x4Buffer));

	m_globalObjects = globalObjects;

	m_size = CFloat4x4Buffer::E_VIEW_COUNT * sizeof(XMFLOAT4X4);

	m_count = CFloat4x4Buffer::E_VIEW_COUNT;

	m_stride = sizeof(XMFLOAT4X4);

	m_commandList = commandList;

	m_constantBuffer = new CConstantBuffer(m_globalObjects, m_commandList, m_size);
}

/*
*/
CFloat4x4Buffer::~CFloat4x4Buffer()
{
	delete m_constantBuffer;
}

/*
*/
void CFloat4x4Buffer::UpdateConstantBuffer()
{
	m_constantBuffer->UpdateBuffer((void*)m_values);
}
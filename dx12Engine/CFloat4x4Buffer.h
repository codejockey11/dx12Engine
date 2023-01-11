#pragma once

#include "standard.h"

#include "CConstantBuffer.h"
#include "CGlobalObjects.h"

class CFloat4x4Buffer
{
public:

	enum
	{
		VIEW_COUNT = 16
	};

	UINT m_size;
	UINT m_count;
	UINT m_stride;

	CConstantBuffer* m_constantBuffer;

	CGlobalObjects* m_globalObjects;

	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	XMFLOAT4X4 m_values[CFloat4x4Buffer::VIEW_COUNT];


	CFloat4x4Buffer();

	CFloat4x4Buffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList);

	~CFloat4x4Buffer();


	void UpdateConstantBuffer();
};
#pragma once

#include "standard.h"

#include "CConstantBuffer.h"
#include "CGlobalObjects.h"

class CIntBuffer
{
public:

	enum
	{
		E_VIEW_COUNT = 64
	};

	CConstantBuffer* m_constantBuffer;
	CGlobalObjects* m_globalObjects;

	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	UINT m_count;
	UINT m_size;
	UINT m_stride;

	int m_values[CIntBuffer::E_VIEW_COUNT];

	CIntBuffer();
	CIntBuffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList);
	~CIntBuffer();

	void UpdateConstantBuffer();
};
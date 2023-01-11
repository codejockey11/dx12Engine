#pragma once

#include "standard.h"

#include "CConstantBuffer.h"
#include "CGlobalObjects.h"

class CIntBuffer
{
public:

	enum
	{
		VIEW_COUNT = 64
	};

	UINT m_size;
	UINT m_count;
	UINT m_stride;

	CConstantBuffer* m_constantBuffer;

	CGlobalObjects* m_globalObjects;

	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	int m_values[CIntBuffer::VIEW_COUNT];


	CIntBuffer();

	CIntBuffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList);

	~CIntBuffer();


	void UpdateConstantBuffer();
};
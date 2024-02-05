#pragma once

#include "standard.h"

#include "CConstantBuffer.h"
#include "CGlobalObjects.h"

class CFloatBuffer
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
	
	float m_values[CFloatBuffer::E_VIEW_COUNT];

	CFloatBuffer();
	CFloatBuffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList);
	~CFloatBuffer();

	void UpdateConstantBuffer();
};
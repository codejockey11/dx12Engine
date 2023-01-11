#pragma once

#include "standard.h"

#include "CConstantBuffer.h"
#include "CGlobalObjects.h"

class CFloatBuffer
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

	float m_values[CFloatBuffer::VIEW_COUNT];


	CFloatBuffer();

	CFloatBuffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList);

	~CFloatBuffer();


	void UpdateConstantBuffer();
};
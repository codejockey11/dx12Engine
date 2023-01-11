#pragma once

#include "standard.h"

#include "CFloatBuffer.h"
#include "CFloat4x4Buffer.h"
#include "CIntBuffer.h"
#include "CGlobalObjects.h"
#include "CTexture.h"

class CShaderResourceView
{
public:

	enum
	{
		TEXTURE = 0,
		BUFFER,

		VIEW_COUNT = 2
	};

	BYTE m_type;

	UINT64 m_slot;

	UINT m_elements;


	CShaderResourceView();

	CShaderResourceView(CGlobalObjects* globalObjects);

	~CShaderResourceView();


	void SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CTexture* texture);

	void SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CFloatBuffer* floatBuffer);

	void SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CFloat4x4Buffer* float4x4Buffer);

	void SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CIntBuffer* intBuffer);

private:

	CGlobalObjects* m_globalObjects;

	D3D12_SHADER_RESOURCE_VIEW_DESC m_desc;
};
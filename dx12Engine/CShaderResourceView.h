#pragma once

#include "standard.h"

#include "CFloat4x4Buffer.h"
#include "CFloatBuffer.h"
#include "CGlobalObjects.h"
#include "CIntBuffer.h"
#include "CTexture.h"

class CShaderResourceView
{
public:

	enum
	{
		E_TEXTURE = 0,
		E_BUFFER,

		E_VIEW_COUNT = 2
	};

	BYTE m_type;

	UINT m_objects;

	UINT64 m_slot;

	CShaderResourceView();
	CShaderResourceView(CGlobalObjects* globalObjects);
	~CShaderResourceView();

	void SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CFloat4x4Buffer* float4x4Buffer);
	void SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CFloatBuffer* floatBuffer);
	void SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CIntBuffer* intBuffer);
	void SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CTexture* texture);

private:

	CGlobalObjects* m_globalObjects;

	D3D12_SHADER_RESOURCE_VIEW_DESC m_desc;
};
#pragma once

#include "standard.h"

#include "CGlobalObjects.h"

class CIndexBuffer
{
public:

	D3D12_INDEX_BUFFER_VIEW	m_view;

	ComPtr<ID3D12Resource> m_indexBuffer;

	UINT m_count;

	ComPtr<ID3D12GraphicsCommandList> m_commandList;


	CIndexBuffer();

	CIndexBuffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList, UINT count, void* indicies);

	~CIndexBuffer();


	BYTE CreateResource();

	void LoadBuffer(void* indices);

	void Draw();

private:

	CGlobalObjects* m_globalObjects;

	D3D12_HEAP_PROPERTIES m_heapProperties;

	D3D12_RESOURCE_DESC m_resourceDesc;
};
#pragma once

#include "standard.h"

#include "CGlobalObjects.h"

class CConstantBuffer
{
public:

	ComPtr<ID3D12Resource> m_buffer;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	D3D12_RESOURCE_BARRIER m_barrier;


	CConstantBuffer();
	CConstantBuffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList, UINT bufferSize);
	~CConstantBuffer();

	void UpdateBuffer(void* values);

private:

	CGlobalObjects* m_globalObjects;
	
	ComPtr<ID3D12Resource> m_uploadHeap;
	
	D3D12_HEAP_PROPERTIES m_bufferHeapProperties;
	D3D12_HEAP_PROPERTIES m_heapProperties;
	
	D3D12_RESOURCE_ALLOCATION_INFO m_allocInfo;
	
	D3D12_RESOURCE_DESC m_bufferDesc;
	D3D12_RESOURCE_DESC m_heapDesc;
	
	D3D12_SUBRESOURCE_DATA m_uploadData;
	
	UINT m_bufferSize;
	
	UINT64 m_uploadBufferSize;
};
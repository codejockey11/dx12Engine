#pragma once

#include "standard.h"

#include "CBmpImage.h"
#include "CErrorLog.h"
#include "CString.h"
#include "CTgaImage.h"
#include "CVideoDevice.h"

class CTexture
{
public:

	UINT64 m_width;

	UINT m_height;

	CTgaImage* m_image;

	CBmpImage* m_bimage;

	bool m_isInitialized;

	ComPtr<ID3D12Resource> m_texture;

	CString* m_name;

	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::BACKBUFFER_COUNT];

	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	D3D12_RESOURCE_BARRIER m_barrier;


	CTexture();

	CTexture(CErrorLog* errorLog, CVideoDevice* videoDevice, const char* filename);

	~CTexture();


	void CreateResource(BYTE* pixels);

private:

	CErrorLog* m_errorLog;

	CVideoDevice* m_videoDevice;

	D3D12_RESOURCE_DESC m_textureDesc;

	D3D12_HEAP_PROPERTIES m_textureHeapProperties;

	D3D12_RESOURCE_DESC m_heapDesc;

	D3D12_HEAP_PROPERTIES m_heapProperties;

	ComPtr<ID3D12Resource> m_textureUploadHeap;

	UINT64 m_uploadBufferSize;

	D3D12_RESOURCE_ALLOCATION_INFO m_allocInfo;

	D3D12_SUBRESOURCE_DATA m_uploadData;
};

class CDynamicTexture : public CTexture
{
public:


	CDynamicTexture(CErrorLog* errorLog, CVideoDevice* videoDevice, ComPtr<ID3D12GraphicsCommandList> commandList, UINT width, UINT height, const char* name);

	~CDynamicTexture();


	void CreateResource();

	void UpdateTextureResource(void* bytes);

private:

	CErrorLog* m_errorLog;

	CVideoDevice* m_videoDevice;

	D3D12_RESOURCE_DESC m_textureDesc;

	D3D12_HEAP_PROPERTIES m_textureHeapProperties;

	D3D12_RESOURCE_DESC m_heapDesc;

	D3D12_HEAP_PROPERTIES m_heapProperties;

	ComPtr<ID3D12Resource> m_textureUploadHeap;

	UINT64 m_uploadBufferSize;

	D3D12_RESOURCE_ALLOCATION_INFO m_allocInfo;

	D3D12_SUBRESOURCE_DATA m_uploadData;
};
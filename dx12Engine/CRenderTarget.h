#pragma once

#include "standard.h"

#include "CErrorLog.h"

class CRenderTarget
{
public:

	ComPtr<ID3D12DescriptorHeap> m_heap;

	D3D12_DESCRIPTOR_HEAP_DESC m_heapDesc;

	D3D12_CPU_DESCRIPTOR_HANDLE	m_handle;

	D2D1_SIZE_F m_rtSize;

	ComPtr<ID3D12Resource> m_resource;

	ComPtr<ID2D1Bitmap1> m_d2dRenderTarget;

	ComPtr<ID3D11Resource> m_wrappedBackBuffer;

	D3D12_RESOURCE_BARRIER m_barrier;

	D3D12_CLEAR_VALUE m_clearValue;


	CRenderTarget();

	CRenderTarget(CErrorLog* errorLog, ComPtr<ID3D12Device> device);

	~CRenderTarget();


	void SwapChainTarget(ComPtr<IDXGISwapChain3> swapChain, UINT bufferNbr);

	void AdditionalTarget(ComPtr<ID3D12Resource> resource, UINT64 width, UINT height);

	void WrapResource(float dpi, ComPtr<ID3D11On12Device> d3d11On12Device, ComPtr<ID2D1DeviceContext1> d2dDeviceContext);

	void SetPreDrawBarrier();

private:

	CErrorLog* m_errorLog;

	ComPtr<ID3D12Device> m_device;

	ComPtr<ID3D11On12Device> m_d3d11On12Device;

	ComPtr<ID2D1DeviceContext1> m_d2dDeviceContext;

	ComPtr<IDXGISurface> m_surface;

	D2D1_BITMAP_PROPERTIES1 m_bitmapProperties;
};
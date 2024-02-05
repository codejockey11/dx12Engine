#pragma once

#include "standard.h"

#include "CColor.h"
#include "CD3D11On12.h"
#include "CDepthBuffer.h"
#include "CErrorLog.h"
#include "CRenderTarget.h"
#include "CViewport.h"
#include "CWindow.h"

class CVideoDevice
{
public:

	enum
	{
		E_BACKBUFFER_COUNT = 2,
		E_DISPLAY_MODE_COUNT = 256
	};
	
	CD3D11On12* m_d3d11On12;
	CDepthBuffer* m_swapChainDepthBuffers[CVideoDevice::E_BACKBUFFER_COUNT];
	CRenderTarget* m_swapChainRenderTargets[CVideoDevice::E_BACKBUFFER_COUNT];
	CViewport* m_swapChainViewport;
	
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::E_BACKBUFFER_COUNT];
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<IDXGISwapChain3> m_swapChain;
	
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS  m_sampleQualityLevels;
	
	UINT m_backbufferIndex;
	UINT m_tearingSupport;

	CVideoDevice();
	CVideoDevice(CErrorLog* errorLog, CWindow* window, CColor* color);
	~CVideoDevice();

	void Create11on12Device();
	ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();
	ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12CommandAllocator> ca);
	void CreateCommandQueue();
	void CreateDepthBuffers();
	void CreateDevice();
	void CreateFence();
	void CreateRenderTargets();
	void CreateSwapChain();
	void DXGIFactory();
	void DeviceInformation();
	void End2DDraw();
	void GetHardwareAdapter();
	void Init2DDraw();
	void MoveToNextFrame();
	void ResetAndClearTargets();
	void ToggleFullScreenWindow();
	void WaitForGPU();

private:

	CColor* m_color;
	CErrorLog* m_errorLog;
	CWindow* m_window;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> m_debugController;
#endif
	ComPtr<ID3D12Fence> m_fence;
	ComPtr<ID3D12InfoQueue> m_infoQueue;
	ComPtr<IDXGIAdapter1> m_adapter;
	ComPtr<IDXGIFactory6> m_factory;

	D3D12_COMMAND_QUEUE_DESC m_commandQueueDesc;

	DXGI_ADAPTER_DESC m_adapterDesc;
	DXGI_MODE_DESC m_displayModeList[CVideoDevice::E_DISPLAY_MODE_COUNT];
	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;

	HANDLE m_fenceEvent;

	RECT m_fullScreenWindowRect;
	RECT m_windowRect;

	UINT m_factoryFlags;
	UINT m_numModes;

	UINT64 m_currentFenceValue;
	UINT64 m_fenceValues[CVideoDevice::E_BACKBUFFER_COUNT];

	bool m_vsyncEnabled;

	int m_videoCardMemory;
};
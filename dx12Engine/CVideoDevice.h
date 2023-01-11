#pragma once

#include "standard.h"

#include "CColor.h"
#include "CDepthBuffer.h"
#include "CD3D11On12.h"
#include "CErrorLog.h"
#include "CRenderTarget.h"
#include "CViewport.h"
#include "CWindow.h"

class CVideoDevice
{
public:

	enum
	{
		BACKBUFFER_COUNT = 4,

		DISPLAY_MODE_COUNT = 256
	};

	
	UINT m_tearingSupport;
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS  m_sampleQualityLevels;

	CD3D11On12* m_d3d11On12;

	CDepthBuffer* m_swapChainDepthBuffers[CVideoDevice::BACKBUFFER_COUNT];

	CRenderTarget* m_swapChainRenderTargets[CVideoDevice::BACKBUFFER_COUNT];

	CViewport* m_swapChainViewport;

	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::BACKBUFFER_COUNT];

	ComPtr<ID3D12CommandQueue> m_commandQueue;

	ComPtr<ID3D12Device> m_device;

	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	ComPtr<IDXGISwapChain3> m_swapChain;

	UINT m_backbufferIndex;


	CVideoDevice();

	CVideoDevice(CErrorLog* errorLog, CWindow* window);

	~CVideoDevice();


	ComPtr<ID3D12CommandAllocator> CreateCommandAllocator();

	ComPtr<ID3D12GraphicsCommandList> CreateCommandList(ComPtr<ID3D12CommandAllocator> ca);


	void ResetAndClearTargets();
	void MoveToNextFrame();
	void CreateFence();
	void WaitForGPU();
	void Init2DDraw();
	void End2DDraw();
	void ToggleFullscreenWindow();

private:

	CErrorLog* m_errorLog;

	CWindow* m_window;

#ifdef _DEBUG
	ComPtr<ID3D12Debug> m_debugController;
#endif

	bool m_vsyncEnabled;

	D3D12_COMMAND_QUEUE_DESC m_commandQueueDesc;

	DXGI_ADAPTER_DESC m_adapterDesc;

	DXGI_MODE_DESC m_displayModeList[CVideoDevice::DISPLAY_MODE_COUNT];

	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;

	HANDLE m_fenceEvent;

	ComPtr<ID3D12Fence> m_fence;

	ComPtr<IDXGIAdapter1> m_adapter;

	ComPtr<IDXGIFactory6> m_factory;

	int m_videoCardMemory;

	RECT m_fullscreenWindowRect;

	RECT m_windowRect;

	UINT m_factoryFlags;

	UINT m_numModes;

	UINT64 m_currentFenceValue;

	UINT64 m_fenceValues[CVideoDevice::BACKBUFFER_COUNT];

	ComPtr<ID3D12InfoQueue> m_infoQueue;


	void DXGIFactory();
	void GetHardwareAdapter();
	void CreateDevice();
	void CreateCommandQueue();
	void DeviceInformation();
	void CreateSwapChain();
	void Create11on12Device();
	void CreateRenderTargets();
	void CreateDepthBuffers();
};
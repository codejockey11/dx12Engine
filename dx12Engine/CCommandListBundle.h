#pragma once

#include "standard.h"

#include "CErrorLog.h"
#include "CVideoDevice.h"

class CCommandListBundle
{
public:

	enum
	{
		MAX_COMMAND_LIST_COUNT = 16
	};

	ID3D12CommandList* m_commandListPointers[CCommandListBundle::MAX_COMMAND_LIST_COUNT];

	ComPtr<ID3D12GraphicsCommandList> m_commandLists[CCommandListBundle::MAX_COMMAND_LIST_COUNT];


	CCommandListBundle();

	CCommandListBundle(CErrorLog* errorLog, CVideoDevice* device);

	~CCommandListBundle();


	void Add(ComPtr<ID3D12GraphicsCommandList> commandList);

	void Clear();

	void Close();

	void Execute();

private:

	CErrorLog* m_errorLog;

	CVideoDevice* m_videoDevice;

	UINT m_count;
};


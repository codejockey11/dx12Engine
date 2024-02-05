#include "CLine.h"

/*
*/
CLine::CLine()
{
	memset(this, 0x00, sizeof(CLine));
}

/*
*/
CLine::CLine(CGlobalObjects* globalObjects, CVertex* from, CVertex* to)
{
	memset(this, 0x00, sizeof(CLine));

	m_globalObjects = globalObjects;

	m_texture = m_globalObjects->m_textureMgr->Create("red.bmp");

	for (UINT i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		m_commandAllocators[i] = m_globalObjects->m_videoDevice->CreateCommandAllocator();
	}

	m_commandList = m_globalObjects->m_videoDevice->CreateCommandList(m_commandAllocators[0]);

	m_commandList->SetName(L"CLine::CLine");

	m_line[0].p = from->p;
	m_line[1].p = to->p;

	m_vertexBuffer = new CVertexBuffer(m_globalObjects, m_commandList, CVertex::E_VT_VERTEXLINE, 2, m_line);


	m_wvp = new CFloat4x4Buffer(m_globalObjects, m_commandList);

	m_shaderHeap = new CShaderHeap(m_globalObjects, m_commandList, 1);

	m_shaderHeap->SlotResource(0, m_wvp);

	
	m_range = new CRange();

	m_range->SetRange(0, D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1);

	m_rootSignature = new CRootSignature(m_globalObjects, 1, m_range);

	m_pipelineState = new CPipelineState(m_globalObjects, CVertex::E_VT_VERTEXLINE);

	m_vertexShader = m_globalObjects->m_shaderMgr->Create("vertexModel.hlsl", "VSMain", "vs_5_0");
	m_pixelShader = m_globalObjects->m_shaderMgr->Create("pixelModel.hlsl", "PSMain", "ps_5_0");

	m_pipelineState->Create(false, false, true,
		D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE,
		m_vertexShader, m_pixelShader, nullptr, nullptr, nullptr,
		m_rootSignature->m_signature);
}

/*
*/
CLine::~CLine()
{
	delete m_pipelineState;
	delete m_rootSignature;
	delete m_range;
	delete m_shaderHeap;
	delete m_wvp;
	delete m_vertexBuffer;

	m_commandList.Reset();

	for (int i = 0; i < CVideoDevice::E_BACKBUFFER_COUNT; i++)
	{
		m_commandAllocators[i].Reset();
	}
}

/*
*/
void CLine::Draw()
{
	m_commandAllocators[m_globalObjects->m_videoDevice->m_backbufferIndex]->Reset();

	m_commandList->Reset(m_commandAllocators[m_globalObjects->m_videoDevice->m_backbufferIndex].Get(), nullptr);


	m_commandList->RSSetViewports(1, &m_globalObjects->m_videoDevice->m_swapChainViewport->m_viewport);

	m_commandList->RSSetScissorRects(1, &m_globalObjects->m_videoDevice->m_swapChainViewport->m_scissorRect);

	m_commandList->OMSetRenderTargets(1,
		&m_globalObjects->m_videoDevice->m_swapChainRenderTargets[m_globalObjects->m_videoDevice->m_backbufferIndex]->m_handle,
		false,
		&m_globalObjects->m_videoDevice->m_swapChainDepthBuffers[m_globalObjects->m_videoDevice->m_backbufferIndex]->m_handle);


	m_commandList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	m_commandList->SetGraphicsRootSignature(m_rootSignature->m_signature.Get());

	m_commandList->SetPipelineState(m_pipelineState->m_pipelineState.Get());


	m_wvp->UpdateConstantBuffer();


	m_commandList->SetDescriptorHeaps(1, m_shaderHeap->m_heap.GetAddressOf());

	m_commandList->SetGraphicsRootDescriptorTable(0, m_shaderHeap->m_heap->GetGPUDescriptorHandleForHeapStart());

	
	m_vertexBuffer->Draw();
}
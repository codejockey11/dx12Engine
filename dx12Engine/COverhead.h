#pragma once

#include "standard.h"

#include "CCamera.h"
#include "CDepthBuffer.h"
#include "CFloatBuffer.h"
#include "CGlobalObjects.h"
#include "CIndexBuffer.h"
#include "CPipelineState.h"
#include "CRange.h"
#include "CRenderTarget.h"
#include "CRootSignature.h"
#include "CShader.h"
#include "CShaderHeap.h"
#include "CTexture.h"
#include "CVertex.h"
#include "CVertexBuffer.h"
#include "CViewport.h"

class COverhead
{
public:

	CCamera* m_camera;
	CDepthBuffer* m_depthBuffers[CVideoDevice::E_BACKBUFFER_COUNT];
	CDynamicTexture* m_texture;
	CFloat4x4Buffer* m_wvp;
	CFloatBuffer* m_floats;
	CIntBuffer* m_ints;
	CPipelineState* m_pipelineState;
	CRange* m_range;
	CRenderTarget* m_renderTargets[CVideoDevice::E_BACKBUFFER_COUNT];
	CRootSignature* m_rootSignature;
	CShader* m_pixelShader;
	CShader* m_vertexShader;
	CShaderHeap* m_shaderHeap;
	CVertex2 m_position;
	CVertex2 m_size;
	CViewport* m_viewport;
	
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::E_BACKBUFFER_COUNT];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	ComPtr<ID3D12Resource> m_resources[CVideoDevice::E_BACKBUFFER_COUNT];

	COverhead();
	COverhead(CGlobalObjects* globalObjects, CVertex2 size, CVertex2 position);
	~COverhead();

	void ClearTargets();
	void Record();
	void SetCameraPosition(XMFLOAT3 position);
	void UpdatePosition();

private:

	CGlobalObjects* m_globalObjects;
	CIndexBuffer* m_indexBuffer;
	CVertexBuffer* m_vertexBuffer;
	CVertexT m_vertices[4];
};
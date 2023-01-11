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

	CRenderTarget* m_renderTargets[CVideoDevice::BACKBUFFER_COUNT];

	CDepthBuffer* m_depthBuffers[CVideoDevice::BACKBUFFER_COUNT];

	CViewport* m_viewport;

	CCamera* m_camera;

	ComPtr<ID3D12Resource> m_resources[CVideoDevice::BACKBUFFER_COUNT];

	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::BACKBUFFER_COUNT];

	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	CPipelineState* m_pipelineState;

	CRange* m_range;

	CRootSignature* m_rootSignature;

	CShaderHeap* m_shaderHeap;

	CFloat4x4Buffer* m_wvp;

	CFloatBuffer* m_floats;

	CIntBuffer* m_ints;


	CShader* m_vertexShader;
	CShader* m_pixelShader;

	CDynamicTexture* m_texture;

	CVertex2 m_size;
	CVertex2 m_position;


	COverhead();

	COverhead(CGlobalObjects* globalObjects, CVertex2 size, CVertex2 position);

	~COverhead();


	void ClearTargets();

	void SetCameraPosition(XMFLOAT3 position);

	void UpdatePosition();

	void Record();

private:

	CGlobalObjects* m_globalObjects;

	CVertexBuffer* m_vertexBuffer;

	CVertexT m_vertices[4];

	CIndexBuffer* m_indexBuffer;
};
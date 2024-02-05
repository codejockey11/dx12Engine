#pragma once

#include "standard.h"

#include "CFloatBuffer.h"
#include "CGlobalObjects.h"
#include "CMaterial.h"
#include "CPipelineState.h"
#include "CRange.h"
#include "CRootSignature.h"
#include "CShader.h"
#include "CShaderHeap.h"
#include "CVertexBuffer.h"

class CLine
{
public:

	CFloat4x4Buffer* m_wvp;
	CPipelineState* m_pipelineState;
	CRange* m_range;
	CRootSignature* m_rootSignature;
	CShader* m_pixelShader;
	CShader* m_vertexShader;
	CShaderHeap* m_shaderHeap;

	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::E_BACKBUFFER_COUNT];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	CLine();
	CLine(CGlobalObjects* globalObjects, CVertex* from, CVertex* to);
	~CLine();

	void Draw();

private:

	CGlobalObjects* m_globalObjects;
	CTexture* m_texture;
	CVertex m_line[2];
	CVertexBuffer* m_vertexBuffer;
};
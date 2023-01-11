#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CVertexBuffer.h"
#include "CPipelineState.h"
#include "CRange.h"
#include "CRootSignature.h"
#include "CShader.h"
#include "CShaderHeap.h"
#include "CFloatBuffer.h"
#include "CMaterial.h"

class CLine
{
public:

	CFloat4x4Buffer* m_wvp;

	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::BACKBUFFER_COUNT];

	CPipelineState* m_pipelineState;

	CRange* m_range;

	CRootSignature* m_rootSignature;

	CShaderHeap* m_shaderHeap;

	CShader* m_vertexShader;

	CShader* m_pixelShader;


	CLine();

	CLine(CGlobalObjects* globalObjects, CVertex* from, CVertex* to);

	~CLine();


	void Draw();

private:

	CGlobalObjects* m_globalObjects;

	CTexture* m_texture;

	CVertexBuffer* m_vertexBuffer;

	CVertex m_line[2];
};
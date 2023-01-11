#pragma once

#include "standard.h"

#include "CBoundBox.h"
#include "CFloat4x4Buffer.h"
#include "CGlobalObjects.h"
#include "CIndexBuffer.h"
#include "CRange.h"
#include "CRootSignature.h"
#include "CShaderHeap.h"
#include "CTexture.h"
#include "CVertexBuffer.h"

class CImage
{
public:

	CVertex2 m_position;
	CVertex2 m_size;
	CVertex2 m_scaleSize;

	CBoundBox* m_box;

	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::BACKBUFFER_COUNT];

	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	CPipelineState* m_pipelineState;

	CRange* m_range;

	CRootSignature* m_rootSignature;

	CShaderHeap* m_shaderHeap;

	CShader* m_vertexShader;
	CShader* m_pixelShader;

	CFloat4x4Buffer* m_wvp;

	CFloatBuffer* m_floats;

	CIntBuffer* m_ints;

	CVertexT m_vertices[4];


	CImage();

	CImage(CGlobalObjects* globalObjects, CTexture* texture, CVertex2 size, CVertex2 position);

	~CImage();


	void UpdatePosition();

	void Draw();

private:

	CGlobalObjects* m_globalObjects;

	CTexture* m_texture;

	CVertex2 m_sheetSize;
	CVertex2 m_cellSize;

	CVertexBuffer* m_vertexBuffer;

	CIndexBuffer* m_indexBuffer;

	DWORD m_color;

	float m_cl;
	float m_ct;
	float m_cr;
	float m_cb;
};
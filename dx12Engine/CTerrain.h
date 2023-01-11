#pragma once

#include "standard.h"

#include "CCamera.h"
#include "CCommandListBundle.h"
#include "CFloatBuffer.h"
#include "CFloat4x4Buffer.h"
#include "CGlobalObjects.h"
#include "CHeapArray.h"
#include "CIndexBuffer.h"
#include "CLinkList.h"
#include "CMaterial.h"
#include "CPipelineState.h"
#include "CRange.h"
#include "CRootSignature.h"
#include "CShader.h"
#include "CShaderHeap.h"
#include "CTerrainClip.h"
#include "CTerrainTile.h"
#include "CVertexBuffer.h"

class CTerrain
{
public:

	CFloat4x4Buffer* m_wvp;

	CFloatBuffer* m_floats;

	CIntBuffer* m_ints;

	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::BACKBUFFER_COUNT];

	CPipelineState* m_pipelineState;

	CRange* m_range;

	CRootSignature* m_rootSignature;

	CShaderHeap* m_shaderHeap;

	CShader* m_vertexShader;

	CShader* m_pixelShader;

	CHeapArray* m_clips;

	UINT m_width;
	
	UINT m_height;
	
	UINT m_primSize;

	CString* m_name;


	CTerrain();

	CTerrain(CGlobalObjects* globalObjects, const char* filename);

	~CTerrain();

	
	void SetCurrentCamera(CCamera* camera);

	void Record();

private:

	CGlobalObjects* m_globalObjects;

	CTerrainTile* m_tile;
	
	CVertexBuffer* m_vertexBuffer;

	CIndexBuffer* m_indexBuffer;

	int m_size;

	CTexture* m_texture0;
	CTexture* m_texture1;
	CTexture* m_texture2;
	CTexture* m_texture3;
	CTexture* m_texture4;
	CTexture* m_texture5;
};
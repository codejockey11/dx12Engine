#pragma once

#include "standard.h"

#include "CCamera.h"
#include "CCommandListBundle.h"
#include "CFloat4x4Buffer.h"
#include "CFloatBuffer.h"
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
#include "CTerrainCollision.h"
#include "CTerrainTile.h"
#include "CVertexBuffer.h"

class CTerrain
{
public:

	CFloat4x4Buffer* m_wvp;
	CFloatBuffer* m_floats;
	CIntBuffer* m_ints;
	CPipelineState* m_pipelineState;
	CRange* m_range;
	CRootSignature* m_rootSignature;
	CShader* m_pixelShader;
	CShader* m_vertexShader;
	CShaderHeap* m_shaderHeap;
	CString* m_name;
	
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::E_BACKBUFFER_COUNT];
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	
	UINT m_height;
	UINT m_primSize;
	UINT m_width;

	CTerrain();
	CTerrain(CGlobalObjects* globalObjects, const char* filename);
	~CTerrain();
	
	void Record();
	void SetCurrentCamera(CCamera* camera);

private:

	CGlobalObjects* m_globalObjects;
	CIndexBuffer* m_indexBuffer;
	CTerrainTile* m_tile;
	CTexture* m_texture0;
	CTexture* m_texture1;
	CTexture* m_texture2;
	CTexture* m_texture3;
	CTexture* m_texture4;
	CTexture* m_texture5;
	CVertexBuffer* m_vertexBuffer;
	
	int m_size;
};
#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CHeapArray.h"
#include "CLinkList.h"
#include "COrthoKey.h"
#include "COrthoLock.h"
#include "COrthoTile.h"
#include "CImage.h"
#include "CTexture.h"

class COrthoLevel
{
public:

	UINT m_width;
	UINT m_height;

	CVertex2 m_cellSize;
	
	CVertex2 m_scaleSize;

	COrthoTile* m_redStart[10];

	UINT m_redCount;
	
	COrthoTile* m_blueStart[10];

	UINT m_blueCount;

	CLinkList<COrthoTile>* m_masterTileList;

	CHeapArray* m_tiles;

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

	CString* m_name;


	COrthoLevel();

	COrthoLevel(CGlobalObjects* globalObjects, CTexture* texture, const char* filename, CVertex2* cellSize, CVertex2* sheetSize);

	~COrthoLevel();

	
	void Draw();


private:

	CGlobalObjects* m_globalObjects;

	CTexture* m_dungeonTileset;

	CVertex2 m_position;

	COrthoTile* m_tile;

	COrthoTile* m_lock;
	
	COrthoTile* m_key;
};
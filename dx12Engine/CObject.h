#pragma once

#include "standard.h"

#include "CFloatBuffer.h"
#include "CFrametime.h"
#include "CGlobalObjects.h"
#include "CLinkList.h"
#include "CMaterial.h"
#include "CModel.h"
#include "COverhead.h"
#include "CPipelineState.h"
#include "CRange.h"
#include "CRootSignature.h"
#include "CShader.h"
#include "CShaderHeap.h"
#include "CTimer.h"
#include "CVertexBuffer.h"
#include "CWavefront.h"

class CObject
{
public:

	CFloat4x4Buffer* m_overheadWvp;
	CFloat4x4Buffer* m_wvp;
	CFloatBuffer* m_floats[CMaterial::E_MATERIAL_COUNT];
	CFloatBuffer* m_overheadFloats[CMaterial::E_MATERIAL_COUNT];
	CIntBuffer* m_ints[CMaterial::E_MATERIAL_COUNT];
	CIntBuffer* m_overheadInts[CMaterial::E_MATERIAL_COUNT];
	CShaderHeap* m_overheadHeap[CMaterial::E_MATERIAL_COUNT];
	CShaderHeap* m_shaderHeap[CMaterial::E_MATERIAL_COUNT];
	CString* m_name;
	CTimer* m_limboTimer;
	CVertexBuffer* m_vertexBuffers[CMaterial::E_MATERIAL_COUNT];
	
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	
	XMFLOAT4 m_position;
	XMFLOAT4 m_environmentPosition;
	XMFLOAT4 m_rotation;
	XMFLOAT4 m_scale;
	
	CObject();
	CObject(CGlobalObjects* globalObjects, const char* filename, const char* materialname);
	CObject(CGlobalObjects* globalObjects, UINT descriptorCount);
	CObject(CGlobalObjects* globalObjects, COverhead* overhead, const char* filename,
		const char* vertexShadername, const char* vertexEntrypoint, const char* vertexVersion,
		const char* pixelShaderName, const char* pixelEntryPoint, const char* pixelVersion,
		bool alphaToCoverageEnable, bool blendEnable);
	CObject(CGlobalObjects* globalObjects, COverhead* overhead, const char* filename, const char* materialname,
		const char* vertexShadername, const char* vertexEntrypoint, const char* vertexVersion,
		const char* pixelShaderName, const char* pixelEntryPoint, const char* pixelVersion,
		bool alphaToCoverageEnable, bool blendEnable);
	~CObject();

	void Animation(CFrametime* frametime);
	void CreateDeviceResources(const char* vertexShadername, const char* vertexEntrypoint, const char* vertexVersion,
		const char* pixelShaderName, const char* pixelEntryPoint, const char* pixelVersion,
		bool alphaToCoverageEnable, bool blendEnable);
	void LoadMeshBuffers(CLinkListNode<CMesh>* meshs);
	void Record();
	void SetCurrentCamera(CCamera* camera);
	void SetOverheadCamera(CCamera* camera);
	void SetPosition(float x, float y, float z);
	void SetPosition(CVertex* position);
	void SetRotation(float x, float y, float z);
	void SetRotation(CVertex* position);
	void SetScale(float x, float y, float z);
	void Update();
	void UpdateServer();

private:

	CGlobalObjects* m_globalObjects;
	CLinkList<CMesh>* m_meshs;
	CModel* m_model;
	COverhead* m_overhead;
	CPipelineState* m_pipelineState;
	CRange* m_range;
	CRootSignature* m_rootSignature;
	CShader* m_pixelShader;
	CShader* m_vertexShader;
	CTexture* texture;
	CWavefront* m_wavefront;
	
	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::E_BACKBUFFER_COUNT];

	float m_bob;
	float m_count;
	float m_spin;
};
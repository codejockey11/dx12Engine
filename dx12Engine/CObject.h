#pragma once

#include "standard.h"

#include "CFloatBuffer.h"
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
#include "CVertexBuffer.h"
#include "CWavefront.h"

class CObject
{
public:

	CString* m_name;

	bool isDynamic;

	XMFLOAT4 m_position;
	XMFLOAT4 m_rotation;
	XMFLOAT4 m_scale;

	CFloat4x4Buffer* m_wvp;

	CFloat4x4Buffer* m_overheadWvp;

	// The float and int constant buffers need to parallel the materials
	// otherwise the missing textures to the int buffer will crash the shader
	CFloatBuffer* m_floats[CMaterial::MATERIAL_COUNT];

	CIntBuffer* m_ints[CMaterial::MATERIAL_COUNT];

	CVertexBuffer* m_vertexBuffers[CMaterial::MATERIAL_COUNT];

	CShaderHeap* m_shaderHeap[CMaterial::MATERIAL_COUNT];
	
	// Same as before:
	// The float and int constant buffers need to parallel the materials
	// otherwise the missing texture to the int buffer will crash the shader
	CFloatBuffer* m_overheadFloats[CMaterial::MATERIAL_COUNT];

	CIntBuffer* m_overheadInts[CMaterial::MATERIAL_COUNT];

	CShaderHeap* m_overheadHeap[CMaterial::MATERIAL_COUNT];

	ComPtr<ID3D12GraphicsCommandList> m_commandList;


	CObject();

	CObject(CGlobalObjects* globalObjects, UINT descriptorCount);

	CObject(CGlobalObjects* globalObjects, COverhead* overhead, const char* filename, const char* vertexShadername, const char* vertexEntrypoint, const char* vertexVersion, const char* pixelShaderName, const char* pixelEntryPoint, const char* pixelVersion, bool alphaToCoverageEnable, bool blendEnable);

	CObject(CGlobalObjects* globalObjects, COverhead* overhead, const char* filename, const char* materialname, const char* vertexShadername, const char* vertexEntrypoint, const char* vertexVersion, const char* pixelShaderName, const char* pixelEntryPoint, const char* pixelVersion, bool alphaToCoverageEnable, bool blendEnable);

	~CObject();


	void SetCurrentCamera(CCamera* camera);

	void SetOverheadCamera(CCamera* camera);

	void Update();

	void Record();

private:

	CGlobalObjects* m_globalObjects;

	CPipelineState* m_pipelineState;

	CRange* m_range;

	CRootSignature* m_rootSignature;

	COverhead* m_overhead;

	ComPtr<ID3D12CommandAllocator> m_commandAllocators[CVideoDevice::BACKBUFFER_COUNT];

	CModel* m_model;

	CWavefront* m_wavefront;

	CLinkList<CMesh>* m_meshs;

	CShader* m_vertexShader;

	CShader* m_pixelShader;

	CTexture* texture;


	void CreateDeviceResources(const char* vertexShadername, const char* vertexEntrypoint, const char* vertexVersion, const char* pixelShaderName, const char* pixelEntryPoint, const char* pixelVersion, bool alphaToCoverageEnable, bool blendEnable);

	void LoadMeshBuffers(CLinkListNode<CMesh>* meshs);
};
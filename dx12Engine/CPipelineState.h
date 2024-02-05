#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CVertex.h"

class CPipelineState
{
public:

	ComPtr<ID3D12PipelineState> m_pipelineState;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_desc;

	CPipelineState();
	CPipelineState(CGlobalObjects* globalObjects, BYTE type);
	~CPipelineState();

	void Create(bool alphaToCoverageEnable, bool blendEnable, bool depthEnable, D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType,
		CShader* vertexShader, CShader* pixelShader, CShader* domainShader, CShader* hullShader, CShader* geometryShader,
		ComPtr<ID3D12RootSignature> rootSignature);

private:

	CGlobalObjects* m_globalObjects;

	D3D12_INPUT_ELEMENT_DESC* m_vertexDesc;

	D3D12_RENDER_TARGET_BLEND_DESC m_blendDesc;

	UINT m_vertexDescCount;
};
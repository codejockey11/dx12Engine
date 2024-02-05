#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CRange.h"

class CRootSignature
{
public:

	enum
	{
		STATIC_SAMPLER_COUNT = 2
	};

	ComPtr<ID3D12RootSignature> m_signature;

	CRootSignature();
	CRootSignature(CGlobalObjects* globalObjects, UINT n, CRange* range);
	~CRootSignature();

private:

	CGlobalObjects* m_globalObjects;
	
	ComPtr<ID3DBlob> m_errors;
	ComPtr<ID3DBlob> m_tempSignature;
	
	D3D12_FEATURE_DATA_ROOT_SIGNATURE m_featureData;
	D3D12_ROOT_PARAMETER1 m_rootParameter;
	D3D12_STATIC_SAMPLER_DESC m_sampler[CRootSignature::STATIC_SAMPLER_COUNT];
	D3D12_VERSIONED_ROOT_SIGNATURE_DESC	m_desc;
};
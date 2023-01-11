#pragma once

#include "standard.h"

#include "CFloatBuffer.h"
#include "CFloat4x4Buffer.h"
#include "CGlobalObjects.h"
#include "CIntBuffer.h"
#include "CLinkList.h"
#include "CShaderResourceView.h"
#include "CTexture.h"

class CShaderHeap
{
public:

	ComPtr<ID3D12DescriptorHeap> m_heap;

	CLinkList<CShaderResourceView>* m_resources;


	CShaderHeap();

	CShaderHeap(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList, UINT descriptorCount);

	~CShaderHeap();


	void SlotResource(UINT slot, CTexture* texture);

	void SlotResource(UINT slot, CFloatBuffer* floatBuffer);

	void SlotResource(UINT slot, CFloat4x4Buffer* float4x4Buffer);

	void SlotResource(UINT slot, CIntBuffer* intBuffer);

private:

	CGlobalObjects* m_globalObjects;

	D3D12_DESCRIPTOR_HEAP_DESC m_desc;

	CShaderResourceView* m_float4x4View;

	CShaderResourceView* m_floatView;

	CShaderResourceView* m_intView;

	CShaderResourceView* m_textureView;
};
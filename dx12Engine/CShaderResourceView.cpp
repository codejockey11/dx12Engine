#include "CShaderResourceView.h"

/*
*/
CShaderResourceView::CShaderResourceView()
{
	memset(this, 0x00, sizeof(CShaderResourceView));
}

/*
*/
CShaderResourceView::CShaderResourceView(CGlobalObjects* globalObjects)
{
	memset(this, 0x00, sizeof(CShaderResourceView));

	m_globalObjects = globalObjects;
}

/*
*/
CShaderResourceView::~CShaderResourceView()
{
}

/*
*/
void CShaderResourceView::SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CFloat4x4Buffer* float4x4Buffer)
{
	m_slot = slot;

	handle.ptr += (m_slot *
		m_globalObjects->m_videoDevice->m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	m_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	m_desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
	m_desc.Buffer.FirstElement = 0;
	m_desc.Buffer.NumElements = float4x4Buffer->m_count;
	m_desc.Buffer.StructureByteStride = float4x4Buffer->m_stride;
	m_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;

	m_globalObjects->m_videoDevice->m_device->CreateShaderResourceView(float4x4Buffer->m_constantBuffer->m_buffer.Get(), &m_desc, handle);

	float4x4Buffer->m_constantBuffer->m_buffer->SetName(L"CShaderResourceView::SlotBuffer::CFloat4x4Buffer");
}

/*
*/
void CShaderResourceView::SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CFloatBuffer* floatBuffer)
{
	m_slot = slot;

	handle.ptr += (m_slot *
		m_globalObjects->m_videoDevice->m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	m_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	m_desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
	m_desc.Buffer.FirstElement = 0;
	m_desc.Buffer.NumElements = floatBuffer->m_count;
	m_desc.Buffer.StructureByteStride = floatBuffer->m_stride;
	m_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;

	m_globalObjects->m_videoDevice->m_device->CreateShaderResourceView(floatBuffer->m_constantBuffer->m_buffer.Get(), &m_desc, handle);

	floatBuffer->m_constantBuffer->m_buffer->SetName(L"CShaderResourceView::SlotBuffer::CFloatBuffer");
}

/*
*/
void CShaderResourceView::SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CIntBuffer* intBuffer)
{
	m_slot = slot;

	handle.ptr += (m_slot *
		m_globalObjects->m_videoDevice->m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	m_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_desc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	m_desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
	m_desc.Buffer.FirstElement = 0;
	m_desc.Buffer.NumElements = intBuffer->m_count;
	m_desc.Buffer.StructureByteStride = intBuffer->m_stride;
	m_desc.Buffer.Flags = D3D12_BUFFER_SRV_FLAGS::D3D12_BUFFER_SRV_FLAG_NONE;

	m_globalObjects->m_videoDevice->m_device->CreateShaderResourceView(intBuffer->m_constantBuffer->m_buffer.Get(), &m_desc, handle);

	intBuffer->m_constantBuffer->m_buffer->SetName(L"CShaderResourceView::SlotBuffer::CIntBuffer");
}

/*
*/
void CShaderResourceView::SlotBuffer(D3D12_CPU_DESCRIPTOR_HANDLE handle, UINT64 slot, CTexture* texture)
{
	m_slot = slot;

	handle.ptr += (m_slot *
		m_globalObjects->m_videoDevice->m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

	m_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	m_desc.Format = texture->m_texture->GetDesc().Format;
	m_desc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
	m_desc.Texture2D.MipLevels = 1;

	m_globalObjects->m_videoDevice->m_device->CreateShaderResourceView(texture->m_texture.Get(), &m_desc, handle);

	texture->m_texture->SetName(texture->m_name->GetWText());
}
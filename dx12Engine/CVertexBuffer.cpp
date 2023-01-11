#include "CVertexBuffer.h"

CVertexBuffer::CVertexBuffer()
{
	memset(this, 0x00, sizeof(CVertexBuffer));
}

CVertexBuffer::CVertexBuffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList, BYTE type, UINT count, void* vertices)
{
	memset(this, 0x00, sizeof(CVertexBuffer));

	m_globalObjects = globalObjects;

	m_type = type;

	m_count = count;

	m_commandList = commandList;

	m_matrixYpr = XMMatrixIdentity();

	m_matrixTranslation = XMMatrixIdentity();

	m_matrixScaling = XMMatrixIdentity();


	CVertexBuffer::CreateResource();
	
	CVertexBuffer::LoadBuffer(vertices);
}

CVertexBuffer::~CVertexBuffer()
{
	m_vertexBuffer.Reset();
}

void CVertexBuffer::CreateResource()
{
	// We do not intend to read from this resource on the CPU.
	m_range.Begin = 0;
	m_range.End = 0;

	m_heapProperties.Type = D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
	m_heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY::D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	m_heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL::D3D12_MEMORY_POOL_UNKNOWN;
	m_heapProperties.CreationNodeMask = 1;
	m_heapProperties.VisibleNodeMask = 1;

	m_resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER;
	m_resourceDesc.Alignment = 0;
	m_resourceDesc.Width = 0;
	m_resourceDesc.Height = 1;
	m_resourceDesc.DepthOrArraySize = 1;
	m_resourceDesc.MipLevels = 1;
	m_resourceDesc.Format = DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
	m_resourceDesc.SampleDesc.Count = 1;
	m_resourceDesc.SampleDesc.Quality = 0;  // 0 for vertex buffer
	m_resourceDesc.Layout = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	m_resourceDesc.Flags = D3D12_RESOURCE_FLAGS::D3D12_RESOURCE_FLAG_NONE;

	switch (m_type)
	{
	case CVertex::VT_VERTEXRGBA:
	{
		m_resourceDesc.Width = m_count * sizeof(CVertexRGBA);

		HRESULT hr = m_globalObjects->m_videoDevice->m_device->CreateCommittedResource(&m_heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &m_resourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertexBuffer));

		if (hr < 0)
		{
			m_globalObjects->m_errorLog->WriteError("CVertexBuffer::CVertexBuffer::CreateCommittedResource::CVertex::VT_VERTEXRGBA:%s\n", m_globalObjects->m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText());

			return;
		}

		m_vertexBuffer->SetName(L"CVertex::VT_VERTEXRGBA");

		m_staticBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_staticBufferView.StrideInBytes = sizeof(CVertexRGBA);
		m_staticBufferView.SizeInBytes = m_count * sizeof(CVertexRGBA);

		break;
	}

	case CVertex::VT_VERTEXT:
	{
		m_resourceDesc.Width = m_count * sizeof(CVertexT);

		HRESULT hr = m_globalObjects->m_videoDevice->m_device->CreateCommittedResource(&m_heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &m_resourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertexBuffer));

		if (hr < 0)
		{
			m_globalObjects->m_errorLog->WriteError("CVertexBuffer::CVertexBuffer::CreateCommittedResource::CVertex::VT_VERTEXT:%s\n", m_globalObjects->m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText());

			return;
		}

		m_vertexBuffer->SetName(L"CVertex::VT_VERTEXT");

		m_staticBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_staticBufferView.StrideInBytes = sizeof(CVertexT);
		m_staticBufferView.SizeInBytes = m_count * sizeof(CVertexT);

		break;
	}

	case CVertex::VT_VERTEXLINE:
	case CVertex::VT_VERTEXNT:
	{
		m_resourceDesc.Width = m_count * sizeof(CVertexNT);

		HRESULT hr = m_globalObjects->m_videoDevice->m_device->CreateCommittedResource(&m_heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &m_resourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertexBuffer));

		if (hr < 0)
		{
			m_globalObjects->m_errorLog->WriteError("CVertexBuffer::CVertexBuffer::CreateCommittedResource::CVertex::VT_VERTEXNT:%s\n", m_globalObjects->m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText());

			return;
		}

		m_vertexBuffer->SetName(L"CVertex::VT_VERTEXNT");

		m_staticBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_staticBufferView.StrideInBytes = sizeof(CVertexNT);
		m_staticBufferView.SizeInBytes = m_count * sizeof(CVertexNT);

		break;
	}

	case CVertex::VT_VERTEX2D:
	{
		m_resourceDesc.Width = m_count * sizeof(CVertex2d);

		HRESULT hr = m_globalObjects->m_videoDevice->m_device->CreateCommittedResource(&m_heapProperties, D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE, &m_resourceDesc, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vertexBuffer));

		if (hr < 0)
		{
			m_globalObjects->m_errorLog->WriteError("CVertexBuffer::CVertexBuffer::CreateCommittedResource::CVertex::VT_VERTEX2D:%s\n", m_globalObjects->m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText());

			return;
		}

		m_vertexBuffer->SetName(L"CVertex::VT_VERTEX2D");

		m_staticBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
		m_staticBufferView.StrideInBytes = sizeof(CVertex2d);
		m_staticBufferView.SizeInBytes = m_count * sizeof(CVertex2d);

		break;
	}

	default:
	{
		m_globalObjects->m_errorLog->WriteError("CVertexBuffer::LoadBuffer::Unknown CVertex::<type>\n");

		return;

		break;
	}
	}
}

void CVertexBuffer::LoadBuffer(void* vertices)
{
	void* pData;

	switch (m_type)
	{
	case CVertex::VT_VERTEXRGBA:
	{
		m_vertexBuffer->Map(0, nullptr, &pData);
		memcpy(pData, vertices, m_count * sizeof(CVertexRGBA));
		m_vertexBuffer->Unmap(0, nullptr);

		break;
	}

	case CVertex::VT_VERTEXT:
	{
		m_vertexBuffer->Map(0, nullptr, &pData);
		memcpy(pData, vertices, m_count * sizeof(CVertexT));
		m_vertexBuffer->Unmap(0, nullptr);

		break;
	}

	case CVertex::VT_VERTEXNT:
	{
		m_vertexBuffer->Map(0, nullptr, &pData);
		memcpy(pData, vertices, m_count * sizeof(CVertexNT));
		m_vertexBuffer->Unmap(0, nullptr);

		break;
	}

	case CVertex::VT_VERTEX2D:
	{
		m_vertexBuffer->Map(0, nullptr, &pData);
		memcpy(pData, vertices, m_count * sizeof(CVertex2d));
		m_vertexBuffer->Unmap(0, nullptr);

		break;
	}

	default:
	{
		m_globalObjects->m_errorLog->WriteError("CVertexBuffer::LoadBuffer::Unknown CVertex::<type>\n");

		return;

		break;
	}
	}
}

void CVertexBuffer::Draw()
{
	m_commandList->IASetVertexBuffers(0, 1, &m_staticBufferView);

	m_commandList->DrawInstanced(m_count, 1, 0, 0);
}

void CVertexBuffer::DrawIndexed(int c)
{
	m_commandList->IASetVertexBuffers(0, 1, &m_staticBufferView);

	m_commandList->DrawIndexedInstanced(c, 1, 0, 0, 0);
}

void CVertexBuffer::UpdateRotation()
{
	m_yaw = m_rotation.y * PiDiv180;
	m_pitch = m_rotation.x * PiDiv180;
	m_roll = m_rotation.z * PiDiv180;

	m_matrixScaling = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);

	m_matrixYpr = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);

	m_matrixTranslation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	m_matrixYpr = XMMatrixMultiply(XMMatrixMultiply(m_matrixYpr, m_matrixScaling), m_matrixTranslation);
}

void CVertexBuffer::Update(void* vertices)
{
	switch (m_type)
	{
	case CVertex::VT_VERTEXRGBA:
	{
		CVertexRGBA* pData = {};

		CVertexRGBA* vertice = (CVertexRGBA*)vertices;

		m_vertexBuffer->Map(0, nullptr, (void**)&pData);

		for (UINT i = 0; i < m_count; i++)
		{
			FXMVECTOR vp = XMLoadFloat3(&vertice->p);

			XMVECTOR v = XMVector3TransformCoord(vp, m_matrixYpr);

			pData->p = XMFLOAT3(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));

			pData++;
			vertice++;
		}

		m_vertexBuffer->Unmap(0, nullptr);

		break;
	}
	case CVertex::VT_VERTEXT:
	{
		CVertexT* pData = {};

		CVertexT* vertice = (CVertexT*)vertices;

		m_vertexBuffer->Map(0, nullptr, (void**)&pData);

		for (UINT i = 0; i < m_count; i++)
		{
			FXMVECTOR vp = XMLoadFloat3(&vertice->p);

			XMVECTOR v = XMVector3TransformCoord(vp, m_matrixYpr);

			pData->p = XMFLOAT3(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));

			pData++;
			vertice++;
		}

		m_vertexBuffer->Unmap(0, nullptr);

		break;
	}
	case CVertex::VT_VERTEXNT:
	{
		CVertexNT* pData = {};

		CVertexNT* vertice = (CVertexNT*)vertices;

		m_vertexBuffer->Map(0, nullptr, (void**)&pData);

		for (UINT i = 0; i < m_count; i++)
		{
			FXMVECTOR vp = XMLoadFloat3(&vertice->p);

			XMVECTOR v = XMVector3TransformCoord(vp, m_matrixYpr);

			pData->p = XMFLOAT3(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));

			FXMVECTOR vn = XMLoadFloat3(&vertice->n);

			v = XMVector3TransformCoord(vn, m_matrixYpr);

			pData->n = XMFLOAT3(XMVectorGetX(v), XMVectorGetY(v), XMVectorGetZ(v));

			pData++;
			vertice++;
		}

		m_vertexBuffer->Unmap(0, nullptr);

		break;
	}
	}
}
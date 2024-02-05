#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CPipelineState.h"
#include "CVertex.h"

class CVertexBuffer
{
public:

	ComPtr<ID3D12GraphicsCommandList> m_commandList;
	
	XMFLOAT4 m_position;
	XMFLOAT4 m_rotation;
	XMFLOAT4 m_scale;
	
	XMMATRIX m_matrixFinal;
	XMMATRIX m_matrixScaling;
	XMMATRIX m_matrixTranslation;
	XMMATRIX m_matrixYpr;
	
	float m_pitch;
	float m_roll;
	float m_yaw;

	CVertexBuffer();
	CVertexBuffer(UINT count, void* vertices);
	CVertexBuffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList, BYTE type, UINT count, void* vertices);
	~CVertexBuffer();

	void CreateResource();
	void Draw();
	void DrawIndexed(int count);
	void LoadBuffer(void* vertices);
	void Update(void* vertices);
	void UpdateRotation();
	void UpdateServer(void* vertices);

private:

	BYTE m_type;
	
	CGlobalObjects* m_globalObjects;
	CVertexNT* m_serverVertices;

	ComPtr<ID3D12Resource> m_vertexBuffer;
	
	D3D12_HEAP_PROPERTIES m_heapProperties;
	D3D12_RANGE m_range;
	D3D12_RESOURCE_DESC m_resourceDesc;
	D3D12_VERTEX_BUFFER_VIEW m_dynamicBufferView;
	D3D12_VERTEX_BUFFER_VIEW m_staticBufferView;
	
	UINT m_count;
};
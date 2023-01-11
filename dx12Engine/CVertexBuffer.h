#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CPipelineState.h"
#include "CVertex.h"

class CVertexBuffer
{
public:

	float m_yaw;
	float m_pitch;
	float m_roll;

	XMFLOAT4 m_position;
	XMFLOAT4 m_rotation;
	XMFLOAT4 m_scale;

	XMMATRIX m_matrixScaling;
	XMMATRIX m_matrixYpr;
	XMMATRIX m_matrixTranslation;

	ComPtr<ID3D12GraphicsCommandList> m_commandList;


	CVertexBuffer();

	CVertexBuffer(CGlobalObjects* globalObjects, ComPtr<ID3D12GraphicsCommandList> commandList, BYTE type, UINT count, void* vertices);

	~CVertexBuffer();


	void LoadBuffer(void* vertices);

	void Draw();

	void DrawIndexed(int count);

	void UpdateRotation();

	void Update(void* vertices);

private:

	CGlobalObjects* m_globalObjects;

	ComPtr<ID3D12Resource> m_vertexBuffer;

	// Static models will update the master buffer one time after being loaded
	D3D12_VERTEX_BUFFER_VIEW m_staticBufferView;

	// The dynamic buffer view will be used for rendering objects after positions and rotations have been applied
	// It is the output of the m_staticBufferView after positions and rotations have been applied
	// When using dynamic buffers this buffer will be used for rendering
	D3D12_VERTEX_BUFFER_VIEW m_dynamicBufferView;

	D3D12_HEAP_PROPERTIES m_heapProperties;

	D3D12_RESOURCE_DESC m_resourceDesc;

	D3D12_RANGE m_range;

	BYTE m_type;

	UINT m_count;


	void CreateResource();
};
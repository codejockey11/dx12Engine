#ifndef _CConstantBuffer
#define _CConstantBuffer

#include "standard.h"

#include "CGlobalObjects.h"

class CConstantBuffer
{
public:

	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;

	ID3D12Resource* buffer;

	D3D12_CPU_DESCRIPTOR_HANDLE handle;

	CConstantBuffer();

	CConstantBuffer(CGlobalObjects* g);

	~CConstantBuffer();

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

};
#endif
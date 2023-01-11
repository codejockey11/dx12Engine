#include "CConstantBuffer.h"

CConstantBuffer::CConstantBuffer()
{
	memset(this, 0x00, sizeof(CConstantBuffer));
}

CConstantBuffer::CConstantBuffer(CGlobalObjects* g)
{
	memset(this, 0x00, sizeof(CConstantBuffer));

	globalObjects = g;

}

CConstantBuffer::~CConstantBuffer()
{

}
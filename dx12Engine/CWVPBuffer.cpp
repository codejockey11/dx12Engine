#include "CFloatBuffer.h"

CFloatBuffer::CFloatBuffer()
{
	memset(this, 0x00, sizeof(CFloatBuffer));
}

CFloatBuffer::CFloatBuffer(CGlobalObjects* g)
{
	memset(this, 0x00, sizeof(CFloatBuffer));

	globalObjects = g;

	constantBuffer = new CConstantBuffer(globalObjects, CFloatBuffer::MAX_COUNT, values);
}

CFloatBuffer::~CFloatBuffer()
{
	delete constantBuffer;
}
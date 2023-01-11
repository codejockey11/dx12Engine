#ifndef _CFloatBuffer
#define _CFloatBuffer

#include "standard.h"

#include "CConstantBuffer.h"
#include "CGlobalObjects.h"

class CFloatBuffer
{
public:
	
	enum
	{
		MAX_COUNT = 10
	};

	FLOAT values[CFloatBuffer::MAX_COUNT];

	CConstantBuffer* constantBuffer;

	CFloatBuffer();

	CFloatBuffer(CGlobalObjects* g);

	~CFloatBuffer();

private:

	CGlobalObjects* globalObjects;
};

class CWVPBuffer : public CFloatBuffer
{
public:

	enum
	{
		WORLD = 0,
		VIEW = 16,
		PROJ = 32,

		MAX_COUNT = 48
	};

	FLOAT values[CWVPBuffer::MAX_COUNT];

	CFloatBuffer();

	CFloatBuffer(CGlobalObjects* g);

	~CFloatBuffer();

private:

	CGlobalObjects* globalObjects;
};
#endif
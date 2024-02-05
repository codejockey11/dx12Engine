#ifndef _CBRUSH
#define _CBRUSH

#include "standard.h"

#include "CBrushSide.h"
#include "CGlobalObjects.h"
#include "CVertex.h"
#include "CTimer.h"
#include "CVertexBuffer.h"

class CBrush
{
public:

	CBrushSide* side[6];

	CGlobalObjects* globalObjects;

	CBrush();

	CBrush(CGlobalObjects* g, CVertex* p, CVertex* sz, CVertex2* suv, CVertex2* ouv);

	~CBrush();

	VOID Copy(CBrush* b);

	VOID Draw();
};

class CDBrush : public CBrush
{
public:

	CVertex position;
	CVertex rotation;
	CVertex direction;

	FLOAT rotX;
	FLOAT rotY;
	FLOAT rotZ;
	FLOAT rotA;

	FLOAT pitch;
	FLOAT roll;
	FLOAT yaw;
	FLOAT axis;

	XMFLOAT3 xmfspin;

	CTimer* timer;

	XMFLOAT3 saveVertices[6][4];

	XMFLOAT3 saveNormals[6][4];

	CDBrush();

	CDBrush(CGlobalObjects* g, CVertex* p, CVertex* sz, CVertex2* suv, CVertex2* ouv, UINT ttl);

	~CDBrush();

	VOID UpdateRotation();

	VOID Rotation();
};
#endif
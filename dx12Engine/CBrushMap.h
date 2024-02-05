#ifndef _CBRUSHMAP
#define _CBRUSHMAP

#include "standard.h"

#include "CBrush.h"
#include "CGlobalObjects.h"
#include "CHeapArray.h"

class CBrushMap
{
public:

	CHeapArray* brushes;

	CBrushSide* side[6];

	CEffectManager* effectMgr;

	UINT width;
	UINT height;

	CBrushMap();

	CBrushMap(CGlobalObjects* g, const char* mapName);

	~CBrushMap();

	void Draw();

private:

	CGlobalObjects* globalObjects;

	CBrush* brush;

	BYTE readBuffer;
	char sizeBuffer[3];

	FILE* fp;
	errno_t err;
	size_t br;

	UINT wc;
	UINT hc;
	UINT ww;
	UINT ew;
	UINT nh;
	UINT sh;

	CVertex cubeSize;
	CVertex position;

	CVertex2 textureScale;
	CVertex2 textureOffset;
};
#endif
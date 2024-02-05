#ifndef _CBRUSHSIDE
#define _CBRUSHSIDE

#include "standard.h"

#include "CColor.h"
#include "CGlobalObjects.h"
#include "CIndexBuffer.h"
#include "CLine.h"
#include "CVertexBuffer.h"

class CBrushSide
{
public:

	CVertexNT vertices[4];
	CVertexBuffer* vertexBuffer;

	CTexture* texture;

	CEffect* effect;

	BYTE type;
	BYTE clip;
	BYTE draw;

#ifdef _DEBUG
	CEffect* lineEffect;
	CLine* normalLine[4];
#endif

	CBrushSide();

	CBrushSide(CBrushSide* bs);

	CBrushSide(CGlobalObjects* g, BYTE tp, const CHAR* t, const CHAR* en, CVertex* p, CVertex* sz, CVertex2* suv, CVertex2* ouv);

	~CBrushSide();

	VOID Copy(CBrushSide* bs);

	VOID Draw();

	VOID SetEffectValues();

#ifdef _DEBUG
	VOID UpdateDebug();
#endif
	
	enum
	{
		BBS_TOP = 0,
		BBS_BOTTOM,
		BBS_LEFT,
		BBS_RIGHT,
		BBS_FRONT,
		BBS_BACK,

		MAXBRUSHSIDE = 0xFF
	};

	enum
	{
		SC_CLIP = 0,
		SC_NOCLIP,

		MAXSURFACECLIP = 0xFF
	};

	enum
	{
		SD_DRAW = 0,
		SD_NODRAW,

		MAXSURFACEDRAW = 0xFF
	};

private:

	HRESULT hr;

	CGlobalObjects* globalObjects;

	CIndexBuffer* indexBuffer;

	DWORD index[6];
};
#endif
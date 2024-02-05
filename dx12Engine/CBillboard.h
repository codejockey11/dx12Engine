#ifndef _CBILLBOARD
#define _CBILLBOARD

#include "standard.h"

#include "CGlobalObjects.h"
#include "CIndexBuffer.h"
#include "CLinkList.h"
#include "CVertexBuffer.h"

class CBillboard
{
public:

	CVertex	position;

	CTexture* texture;

	CBillboard();

	CBillboard(CGlobalObjects* g, const CHAR* fn, CVertex2* wh, CVertex* p, BYTE t);

	~CBillboard();

	VOID UpdateVertices();

	VOID Draw();

	VOID SetView(D3DXMATRIX v) { view = v; }

	enum
	{
		BT_NAMEPLATE = 1,
		BT_DRAW,

		MAXBILLBOARDTYPE = 0xFF
	};

private:

	HRESULT	hr;

	CGlobalObjects* globalObjects;

	CEffect* textureEffect;

	CVertex	rightVect;
	CVertex	upVect;

	CVertex	upperLeft;
	CVertex	upperRight;
	CVertex	lowerRight;
	CVertex	lowerLeft;

	CVertex	normal;

	CVertexBuffer* vertexBuffer;
	CIndexBuffer* indexBuffer;

	CVertexNT vertices[4];

	DWORD index[6];

	UINT width;
	UINT height;

	BYTE type;

	D3DXMATRIX view;
};
#endif
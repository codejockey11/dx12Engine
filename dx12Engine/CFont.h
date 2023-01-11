#ifndef _CFONT
#define _CFONT

#include "standard.h"

#include "CBmpImage.h"
#include "CGlobalObjects.h"
#include "CHudImage.h"

#define MAXFONTCOUNT 94

class CFont
{
public:

	CGlobalObjects*	globalObjects;

	CHudImage*		fonts[MAXFONTCOUNT];

	CBmpImage*		bmp;

	CEffect*			effect;

	CFont();

	CFont(CGlobalObjects* g, const CHAR* fn, CVertex2* sz);

	~CFont();

	VOID Draw(const CHAR* text, UINT x, UINT y, CColor* color);

	HRESULT hr;
};
#endif
#ifndef _CFONTMANAGER
#define _CFONTMANAGER

#include "standard.h"

#include "CEffectManager.h"
#include "CErrorLog.h"
#include "CFont.h"
#include "CLinkList.h"
#include "CTextureManager.h"
#include "CVideoDevice.h"
#include "CWindow.h"

class CFontManager
{
public:

	CLinkList* fonts;

	CFontManager();

	CFontManager(CErrorLog* el, CVideoDevice* d, CWindow* w, CEffectManager* em, CTextureManager* tm);

	~CFontManager();

	VOID DeleteCustomFont(const CHAR* fn);

	CFont* MakeCustomFont(const CHAR* face, CVertex2* sz);

	CFont* GetFont(const CHAR* face);

private:

	HRESULT hr;

	CErrorLog* errorLog;

	CVideoDevice* videoDevice;

	CWindow* window;

	CEffectManager* effectMgr;

	CTextureManager* textureMgr;

	CFont* font;
};
#endif
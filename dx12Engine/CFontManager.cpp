#include "CFontManager.h"

CFontManager::CFontManager()
{
	memset(this, 0x00, sizeof(CFontManager));
}

CFontManager::CFontManager(CErrorLog* el, CVideoDevice* d, CWindow* w, CEffectManager* em, CTextureManager* tm)
{
	memset(this, 0x00, sizeof(CFontManager));

	errorLog = el;
	videoDevice = d;
	window = w;
	effectMgr = em;
	textureMgr = tm;

	fonts = new CLinkList();
}

CFontManager::~CFontManager()
{
	delete fonts;
}

CFont* CFontManager::MakeCustomFont(const CHAR* face, CVertex2* sz)
{
	CFont* fc = CFontManager::GetFont(face);

	if (fc)
	{
		return fc;
	}

	font = new CFont(errorLog, videoDevice, effectMgr, textureMgr, face, sz);

	if (font->fonts[0]->texture == NULL)
	{
		errorLog->WriteError("CFontManager::CFont:Failed:%s\n", face);

		delete font;

		return NULL;
	}

	fonts->Add(font, face);

	return font;
}

CFont* CFontManager::GetFont(const CHAR* face)
{
	CLinkListNode* lln = fonts->Search(face);

	if (lln == NULL)
	{
		return NULL;
	}

	return (CFont*)lln->element;
}

VOID CFontManager::DeleteCustomFont(const CHAR* face)
{
	CLinkListNode* lln = fonts->Search(face);

	if (lln)
	{
		fonts->Delete(lln);
	}
}
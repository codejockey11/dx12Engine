#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CLinkList.h"
#include "CButton.h"

class CButtonManager
{
public:

	CLinkList<CButton>* m_buttons;


	CButtonManager();

	CButtonManager(CGlobalObjects* globalObjects);

	~CButtonManager();


	CButton* Create(HMENU hMenu, const char* name, const char* text, CTexture* image, CD12Font* font, CVertex2 size, CVertex2 position, BYTE audience, BYTE type);

	CButton* Get(const char* name);

	void Delete(const char* name);

private:

	CGlobalObjects* m_globalObjects;
};
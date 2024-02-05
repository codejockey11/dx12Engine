#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CVertex.h"

class CWindowsButton
{
public:

	CVertex2 m_fullScreenRatio;
	
	HWND m_button;
	
	POINT m_fullScreenPosition;
	POINT m_fullScreenSize;
	POINT m_position;
	POINT m_size;

	CWindowsButton();
	CWindowsButton(CGlobalObjects* globalObjects, HMENU hMenu, const char* name, CVertex2 size, CVertex2 position);
	~CWindowsButton();

	void FullScreenWindowsButton();
	void WindowWindowsButton();

private:

	CGlobalObjects* m_globalObjects;
};


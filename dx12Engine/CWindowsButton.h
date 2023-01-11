#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CVertex.h"

class CWindowsButton
{
public:

	HWND m_button;

	POINT m_position;
	POINT m_size;
	
	POINT m_fullscreenPosition;
	POINT m_fullscreenSize;

	CVertex2 m_fullscreenRatio;


	CWindowsButton();

	CWindowsButton(CGlobalObjects* globalObjects, HMENU hMenu, const char* name, CVertex2 size, CVertex2 position);
	
	~CWindowsButton();

	void FullscreenWindowsButton();

	void WindowWindowsButton();

private:

	CGlobalObjects* m_globalObjects;
};


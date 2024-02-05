#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CImage.h"
#include "CNetwork.h"
#include "CWindowsButton.h"

class CButton
{
public:

	// Enumeration for Windows WM_COMMAND button controls
	// The button array defined by the user must match
	enum Controls
	{
		E_BTN_BROWSER = 200,
		E_BTN_CONSOLE,
		E_BTN_EXIT,
		E_BTN_WINDOWMODE,
	};

	// Max graphical HUD buttons
	enum
	{
		E_MAX_BUTTONS = 4
	};

	CD12Font* m_font;
	CImage* m_image;
	CNetwork* m_network;
	CSound* m_soundClicked;
	CString* m_name;
	CString* m_text;
	CVertex2 m_position;
	CVertex2 m_size;
	CWindowsButton* m_windowsButton;
	
	bool m_isInitialized;
	bool m_isMouseOver;

	static const int ControlsCount = Controls::E_BTN_BROWSER;

	CButton();
	CButton(CGlobalObjects* globalObjects, HMENU hMenu, const char* name, const char* text,
		CTexture* image, const char* font, float fontSize, CVertex2 size,	CVertex2 position, BYTE audience, BYTE type);
	~CButton();

	void Active();
	void DisplayText();
	void Inactive();
	void Record();

private:

	CGlobalObjects* m_globalObjects;
	
	CSound* m_soundOff;
	CSound* m_soundOn;
	
	bool m_soundOffPlayed;
	bool m_soundOnPlayed;
};
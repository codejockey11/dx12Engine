#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CEvent.h"
#include "CImage.h"
#include "CWindowsButton.h"

class CButton
{
public:

	// Enumeration for Windows WM_COMMAND button controls
	enum Controls
	{
		BTN_CONSOLE = 200,
		BTN_EXIT,
		BTN_WINDOWMODE,
		BTN_BROWSER,
		BTN_ENDLIST
	};

	// Max graphical HUD buttons
	enum
	{
		MAX_BUTTONS = 4
	};

	bool m_isMouseOver;
	bool m_isInitialized;

	CVertex2 m_position;
	CVertex2 m_size;

	CEvent* m_event;

	CImage* m_image;

	CD12Font* m_font;

	CWindowsButton* m_windowsButton;

	CSound* m_soundClicked;

	CString* m_text;

	CString* m_name;

	static const int ControlsCount = Controls::BTN_CONSOLE;


	CButton();

	CButton(CGlobalObjects* globalObjects, HMENU hMenu, const char* name, const char* text, CTexture* image, CD12Font* font, CVertex2 size, CVertex2 position, BYTE audience, BYTE type);

	~CButton();


	void Record();

	void DisplayText();

private:

	CGlobalObjects* m_globalObjects;

	CSound* m_soundOff;
	CSound* m_soundOn;

	bool m_soundOffPlayed;
	bool m_soundOnPlayed;

	void ButtonActive();
	void ButtonInactive();
};
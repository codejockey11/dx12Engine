#pragma once

#include "standard.h"

#include "CD12Font.h"
#include "CEvent.h"
#include "CGlobalObjects.h"
#include "CImage.h"
#include "CString.h"

class CTextField
{
public:

	enum
	{
		MAX_LENGTH = 32
	};

	CString* m_value;

	CImage* m_image;

	CD12Font* m_font;

	CEvent* m_event;

	bool m_isMouseOver;

	bool m_isActive;

	bool m_mask;


	CTextField();

	CTextField(CGlobalObjects* globalObjects, CD12Font* font, CTexture* texture, CVertex2 size, CVertex2 position, BYTE audience, BYTE type, bool mask);

	~CTextField();


	void Record();

	void Backspace();

	void AddChar(const char c);

	CEvent* GetEvent();

	void DisplayText();

private:

	CGlobalObjects* m_globalObjects;

	CSound* m_soundOff;
	CSound* m_soundOn;

	bool m_soundOffPlayed;
	bool m_soundOnPlayed;

	int m_cursorLocation;

	void TextFieldActive();
	void TextFieldInactive();
};


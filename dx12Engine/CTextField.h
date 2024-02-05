#pragma once

#include "standard.h"

#include "CD12Font.h"
#include "CGlobalObjects.h"
#include "CImage.h"
#include "CNetwork.h"
#include "CString.h"

class CTextField
{
public:

	enum
	{
		E_MAX_LENGTH = 32
	};

	CD12Font* m_font;
	CImage* m_image;
	CNetwork* m_network;
	CString* m_value;

	bool m_isActive;
	bool m_isMouseOver;
	bool m_mask;

	CTextField();
	CTextField(CGlobalObjects* globalObjects, CD12Font* font, CTexture* texture, CVertex2 size, CVertex2 position, BYTE audience, BYTE type, bool mask);
	~CTextField();

	void Active();
	void AddChar(const char c);
	void Backspace();
	void DisplayText();
	CNetwork* GetEvent();
	void Inactive();
	void Record();

private:

	CGlobalObjects* m_globalObjects;
	
	CSound* m_soundOff;
	CSound* m_soundOn;
	
	bool m_soundOffPlayed;
	bool m_soundOnPlayed;
	
	int m_cursorLocation;
};


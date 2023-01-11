#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CLinkList.h"
#include "CEvent.h"
#include "CImage.h"
#include "CPlayerInfo.h"
#include "CServer.h"
#include "CString.h"

class CConsoleVars
{
public:

	float m_fog;
	float m_timeOfDay;

	bool m_drawWire;


	CConsoleVars()
	{
		memset(this, 0x00, sizeof(CConsoleVars));

		m_timeOfDay = 1.0f;
	};

	~CConsoleVars() {};
};

class CConsole
{
public:

	enum
	{
		TEXT_LEN = 256,
		PAGE_BACKWARD = 0,
		PAGE_FORWARD = 1
	};

	CString* m_commandLine;
	
	CLinkList<CString>* m_entries;
	
	CConsoleVars* m_variables;

	int m_entryNbr;
	int m_cursorLocation;
	int m_scroll;
	int m_lines;

	CVertex2 m_topLeft;
	CVertex2 m_bottomRight;

	CVertex2 m_size;
	CVertex2 m_position;

	bool m_isVisible;

	CImage* m_background;


	CConsole();

	CConsole(CGlobalObjects* globalObjects, const char* textureName, const char* fontName, float fontSize, float width, float height);

	~CConsole();


	void ToggleVisibility();

	void DisplayConsole();

	void DisplayText();

	void Backspace();

	void AddChar(char c);

	void AddEventMessage(const char* format, ...);

	void Paging(char direction);

	void Reset();

	CEvent* CommandFromConsole(CPlayerInfo* playerInfo);

private:

	CGlobalObjects* m_globalObjects;

	CD12Font* m_font;

	int m_start;

	char m_text[CConsole::TEXT_LEN];

	CEvent* m_event;
};
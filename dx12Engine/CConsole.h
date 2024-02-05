#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CImage.h"
#include "CLinkList.h"
#include "CNetwork.h"
#include "CPlayerInfo.h"
#include "CServer.h"
#include "CString.h"

class CConsole
{
public:

	enum
	{
		E_COMMAND_LENGTH = 32,
		E_ENTRY_LENGTH = 256,
		E_HUGE_BUFFER = 1024,
		E_PAGE_BACKWARD = 0,
		E_PAGE_FORWARD = 1,
		MAX_COMMAND_VARIABLES = 3
	};

	CImage* m_background;
	CLinkList<CString>* m_entries;
	CString* m_commandLine;
	
	CVertex2 m_bottomRight;
	CVertex2 m_position;
	CVertex2 m_size;
	CVertex2 m_topLeft;
	
	bool m_isVisible;
	
	int m_cursorLocation;
	int m_entryNbr;
	int m_lines;
	int m_scroll;

	CConsole();
	CConsole(CGlobalObjects* globalObjects, const char* textureName, const char* fontName, float fontSize, float width, float height);
	~CConsole();

	CNetwork* CommandFromConsole(CServerInfo* serverInfo);
	
	void AddChar(char c);
	void AddEventMessage(const char* format, ...);
	void Backspace();
	void DisplayConsole();
	void DisplayText();
	void Paging(char direction);
	void Reset();
	void ToggleVisibility();

private:

	CD12Font* m_font;
	CGlobalObjects* m_globalObjects;
	
	char m_hugeVsprintBuffer[E_HUGE_BUFFER];
	char m_text[CConsole::E_ENTRY_LENGTH];
	
	int m_start;
};
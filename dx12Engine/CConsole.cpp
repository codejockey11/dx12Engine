#include "CConsole.h"

CConsole::CConsole()
{
	memset(this, 0x00, sizeof(CConsole));
}

CConsole::CConsole(CGlobalObjects* globalObjects, const char* textureName, const char* fontName, float fontSize, float width, float height)
{
	memset(this, 0x00, sizeof(CConsole));

	m_globalObjects = globalObjects;

	m_isVisible = false;


	m_globalObjects->m_fontD12Mgr->Create(fontName, fontSize, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BOLD);

	m_font = m_globalObjects->m_fontD12Mgr->Get(fontName);

	m_globalObjects->m_textureMgr->Create(textureName);

	m_commandLine = new CString(CEvent::DATA_LENGTH);
	m_entries = new CLinkList<CString>();
	m_event = new CEvent();
	m_variables = new CConsoleVars();


	m_position.p.x = (float)(m_globalObjects->m_window->m_width / 2) - (width / 2.0f);
	m_position.p.y = (float)(m_globalObjects->m_window->m_height / 2) - (height / 2.0f);

	m_topLeft.p.x = m_position.p.x;
	m_topLeft.p.y = m_position.p.y;
	m_bottomRight.p.x = m_position.p.x + width;
	m_bottomRight.p.y = m_position.p.y + height;

	m_size.p.x = m_bottomRight.p.x - m_topLeft.p.x + 1.0f;
	m_size.p.y = m_bottomRight.p.y - m_topLeft.p.y + 1.0f;


	m_background = new CImage(m_globalObjects, m_globalObjects->m_textureMgr->Get(textureName), m_size, m_position);

	m_background->m_floats->m_values[3] = 1.0f;
}

CConsole::~CConsole()
{
	delete m_background;
	delete m_variables;
	delete m_event;
	delete m_entries;
	delete m_commandLine;
}

void CConsole::ToggleVisibility()
{
	m_isVisible = !m_isVisible;
}

void CConsole::DisplayConsole()
{
	m_background->Draw();
}

void CConsole::DisplayText()
{
	m_lines = (int)((m_bottomRight.p.y - m_topLeft.p.y) / m_font->m_height) - 1;

	CLinkListNode<CString>* el = m_entries->m_list;

	m_start = (int)(m_bottomRight.p.y - m_font->m_height - (m_font->m_height * m_entries->m_count)) - 4;

	if (m_start < m_topLeft.p.y)
	{
		m_start = (int)(m_bottomRight.p.y - m_font->m_height - (m_font->m_height * m_lines));
	}

	if (m_entries->m_count > m_lines)
	{
		el = m_entries->Search(m_entries->m_count - m_lines + m_scroll);
	}
	else
	{
		el = m_entries->m_list;
	}

	if (el)
	{
		while (el->m_element)
		{
			CVertex2 position(m_topLeft.p.x + 4, (float)m_start);

			m_font->Draw(el->m_element->GetText(), position, m_size, &SunYellow);

			m_start += (int)m_font->m_height;

			if (m_start > (m_bottomRight.p.y - m_font->m_height - m_font->m_height))
			{
				break;
			}

			el = el->m_next;
		}
	}

	CVertex2 position(m_topLeft.p.x + 4, m_bottomRight.p.y - m_font->m_height - 4);

	m_font->Draw(m_commandLine->GetText(), position, m_size, &SunYellow);
}

void CConsole::AddEventMessage(const char* format, ...)
{
	va_list argptr;

	va_start(argptr, format);

	memset((void*)m_text, 0x00, CConsole::TEXT_LEN);

	vsprintf_s(m_text, CConsole::TEXT_LEN, format, argptr);

	va_end(argptr);

	CString* commandList = new CString(m_text);

	m_entries->Add(commandList, m_entryNbr);

	m_entryNbr++;

	CConsole::Reset();
}

void CConsole::Backspace()
{
	m_cursorLocation--;

	if (m_cursorLocation < 0)
	{
		m_cursorLocation = 0;
	}

	m_commandLine->GetText()[m_cursorLocation] = 0x00;
}

void CConsole::AddChar(const char c)
{
	if (m_cursorLocation == m_commandLine->GetLength())
	{
		m_cursorLocation = m_commandLine->GetLength() - 1;

		if (m_cursorLocation < 0)
		{
			m_cursorLocation = 0;
		}
	}

	m_commandLine->GetText()[m_cursorLocation] = (char)c;

	m_cursorLocation++;
}

void CConsole::Paging(char direction)
{
	if (direction == CConsole::PAGE_BACKWARD)
	{
		m_scroll--;
	}

	if ((m_entries->m_count - m_lines + m_scroll) < 0)
	{
		m_scroll++;
	}

	if (direction == CConsole::PAGE_FORWARD)
	{
		m_scroll++;
	}

	if (m_scroll > 0)
	{
		m_scroll = 0;
	}
}

void CConsole::Reset()
{
	m_cursorLocation = 0;

	m_commandLine->Clear();
}

CEvent* CConsole::CommandFromConsole(CPlayerInfo* playerInfo)
{
	if (strlen(m_commandLine->GetText()) == 0)
	{
		return nullptr;
	}

	memset((void*)m_event->m_data, 0x00, CEvent::DATA_LENGTH);

	// chat command
	if (m_commandLine->GetText()[0] != '/')
	{
		m_event->m_fromClient = 0;
		m_event->m_toClient = 0;
		m_event->m_audience = CEvent::ET_SERVER;
		m_event->m_type = CEvent::ET_CHAT;

		m_event->m_length = (UINT)strlen(m_commandLine->GetText());

		if (m_event->m_length > CEvent::DATA_LENGTH)
		{
			m_event->m_length = CEvent::DATA_LENGTH;
		}

		memcpy((void*)m_event->m_data, (void*)m_commandLine->GetText(), m_event->m_length);

		m_event->m_data[CEvent::DATA_LENGTH - 1] = 0x00;

		CConsole::Reset();

		return m_event;
	}

	// simple command with no passed values
	if (strcmp(m_commandLine->GetText(), "/time") == 0)
	{
		struct tm buf;
		char str[26];

		time_t t = time(NULL);

		localtime_s(&buf, &t);
		strftime(str, sizeof(str), "%m:%d:%Y %X", &buf);

		CConsole::AddEventMessage(str);

		return nullptr;
	}

	if (strcmp(m_commandLine->GetText(), "/exit") == 0)
	{
		m_event->m_fromClient = 0;
		m_event->m_toClient = 0;
		m_event->m_audience = CEvent::ET_LOCAL;
		m_event->m_type = CEvent::ET_EXIT;

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return m_event;
	}

	if (strcmp(m_commandLine->GetText(), "/stop") == 0)
	{
		m_event->m_fromClient = 0;
		m_event->m_toClient = 0;
		m_event->m_audience = CEvent::ET_LOCAL;
		m_event->m_type = CEvent::ET_STOPSERVER;

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return m_event;
	}

	if (strcmp(m_commandLine->GetText(), "/disconnect") == 0)
	{
		m_event->m_fromClient = 0;
		m_event->m_toClient = 0;
		m_event->m_audience = CEvent::ET_LOCAL;
		m_event->m_type = CEvent::ET_DISCONNECT;

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return m_event;
	}

	// toggle command
	if (strcmp(m_commandLine->GetText(), "/wire") == 0)
	{
		if (m_variables->m_drawWire)
		{
			m_variables->m_drawWire = false;
		}
		else
		{
			m_variables->m_drawWire = true;
		}

		CConsole::AddEventMessage("%s %x", m_commandLine->GetText(), m_variables->m_drawWire);

		return nullptr;
	}


	// loading varibale c with command name

	UINT i = 0;

	char c[32] = {};

	while ((m_commandLine->GetText()[i] != ' ') && (m_commandLine->GetText()[i] != '\0'))
	{
		memcpy((void*)&c[i], (void*)&m_commandLine->GetText()[i], 1);

		i++;
	}

	i++;

	// loading varibale v with command value

	UINT ii = 0;

	char v[32] = {""};

	while (m_commandLine->GetText()[i] != '\0')
	{
		memcpy((void*)&v[ii], (void*)&m_commandLine->GetText()[i], 1);

		ii++;
		i++;
	}


	// commands with a single value

	if (strcmp(c, "/fog") == 0)
	{
		m_variables->m_fog = (float)atof(v);

		CConsole::AddEventMessage("%s %f", m_commandLine->GetText(), m_variables->m_fog);

		return nullptr;
	}

	if (strcmp(c, "/tod") == 0)
	{
		m_variables->m_timeOfDay = (float)atof(v);

		CConsole::AddEventMessage("%s %f", m_commandLine->GetText(), m_variables->m_timeOfDay);

		return nullptr;
	}

	if (strcmp(c, "/start") == 0)
	{
		m_event->m_fromClient = 0;
		m_event->m_toClient = 0;
		m_event->m_audience = CEvent::ET_LOCAL;
		m_event->m_type = CEvent::ET_STARTSERVER;

		m_event->m_length = (UINT)strlen(v);

		if (m_event->m_length > CEvent::DATA_LENGTH)
		{
			m_event->m_length = CEvent::DATA_LENGTH;
		}

		if (m_event->m_length == 0)
		{
			m_event->m_length = 5;

			memcpy((void*)m_event->m_data, (void*)"26105", m_event->m_length);
		}
		else
		{
			memcpy((void*)m_event->m_data, (void*)v, strlen(v));
		}

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return m_event;
	}

	if (strcmp(c, "/connect") == 0)
	{
		m_event->m_fromClient = 0;
		m_event->m_toClient = 0;
		m_event->m_audience = CEvent::ET_LOCAL;
		m_event->m_type = CEvent::ET_CONNECT;

		m_event->m_length = (UINT)strlen(v);

		if (m_event->m_length > CEvent::DATA_LENGTH)
		{
			m_event->m_length = CEvent::DATA_LENGTH;
		}

		if (m_event->m_length == 0)
		{
			m_event->m_length = 15;

			memcpy((void*)m_event->m_data, (void*)"127.0.0.1:26105", m_event->m_length);
		}
		else
		{
			memcpy((void*)m_event->m_data, (void*)v, strlen(v));
		}

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return m_event;
	}

	if (strcmp(c, "/load") == 0)
	{
		m_event->m_fromClient = 0;
		m_event->m_toClient = 0;
		m_event->m_audience = CEvent::ET_SERVER;
		m_event->m_type = CEvent::CServerEvent::SE_LOADENVIRONMENT;

		m_event->m_length = (UINT)strlen(v);

		if (m_event->m_length > CEvent::DATA_LENGTH)
		{
			m_event->m_length = CEvent::DATA_LENGTH;
		}

		if (m_event->m_length == 0)
		{
			memcpy((void*)m_event->m_data, (void*)"terrain\\vertices.txt", 21);
		}
		else
		{
			memcpy((void*)m_event->m_data, (void*)v, strlen(v));
		}

		memcpy((void*)&m_event->m_playerInfo, (void*)playerInfo, sizeof(CPlayerInfo));

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return m_event;
	}

	CConsole::AddEventMessage("Unknown Command %s", c);

	return nullptr;
}
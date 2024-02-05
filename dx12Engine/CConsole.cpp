#include "CConsole.h"

/*
*/
CConsole::CConsole()
{
	memset(this, 0x00, sizeof(CConsole));
}

/*
*/
CConsole::CConsole(CGlobalObjects* globalObjects, const char* textureName, const char* fontName,
	float fontSize, float width, float height)
{
	memset(this, 0x00, sizeof(CConsole));

	m_globalObjects = globalObjects;

	m_isVisible = false;


	m_globalObjects->m_fontD12Mgr->Create(fontName, fontSize, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BOLD);

	m_font = m_globalObjects->m_fontD12Mgr->Get(fontName);

	m_globalObjects->m_textureMgr->Create(textureName);

	m_commandLine = new CString(CConsole::E_ENTRY_LENGTH);
	
	m_entries = new CLinkList<CString>();


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

/*
*/
CConsole::~CConsole()
{
	delete m_background;
	delete m_entries;
	delete m_commandLine;
}

/*
*/
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

/*
*/
void CConsole::AddEventMessage(const char* format, ...)
{
	va_list argptr;

	va_start(argptr, format);

	memset((void*)m_hugeVsprintBuffer, 0x00, CConsole::E_HUGE_BUFFER);

	vsprintf_s(m_hugeVsprintBuffer, CConsole::E_HUGE_BUFFER, format, argptr);

	va_end(argptr);

	CString* commandList = new CString(m_hugeVsprintBuffer);

	m_entries->Add(commandList, m_entryNbr);

	m_entryNbr++;

	m_globalObjects->m_errorLog->WriteError("%s\n", commandList->GetText());

	CConsole::Reset();
}

/*
*/
void CConsole::Backspace()
{
	m_cursorLocation--;

	if (m_cursorLocation < 0)
	{
		m_cursorLocation = 0;
	}

	m_commandLine->GetText()[m_cursorLocation] = 0x00;
}

/*
*/
CNetwork* CConsole::CommandFromConsole(CServerInfo* serverInfo)
{
	if (strlen(m_commandLine->GetText()) == 0)
	{
		return nullptr;
	}

	// chat command
	if (m_commandLine->GetText()[0] != '/')
	{
		if (serverInfo != nullptr)
		{
			CString* message = new CString(serverInfo->m_playerInfo->m_name);

			message->Concat(":");

			message->Concat(m_commandLine->GetText());

			CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_CHAT,
				(void*)message->GetText(), message->GetLength(),
				(void*)serverInfo->m_playerInfo,
				(void*)serverInfo);

			delete message;

			CConsole::Reset();

			return network;
		}

		CConsole::Reset();

		return nullptr;
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
		CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_EXIT_GAME,
			nullptr, 0,
			(void*)serverInfo->m_playerInfo,
			nullptr);

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return network;
	}

	if (strcmp(m_commandLine->GetText(), "/stop") == 0)
	{
		CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_STOP_SERVER,
			nullptr, 0,
			(void*)serverInfo->m_playerInfo,
			nullptr);

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return network;
	}

	if (strcmp(m_commandLine->GetText(), "/disconnect") == 0)
	{
		CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_DISCONNECT,
			nullptr, 0,
			(void*)serverInfo->m_playerInfo,
			nullptr);

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return network;
	}

	// Parse the command line for potential variables
	char command[CConsole::MAX_COMMAND_VARIABLES][CConsole::E_COMMAND_LENGTH] = {};

	size_t start = 0;

	for (int i = 0; i < CConsole::MAX_COMMAND_VARIABLES; i++)
	{
		sscanf_s(&m_commandLine->GetText()[start], "%s", &command[i], CConsole::E_COMMAND_LENGTH);

		start += strlen(command[i]) + 1;
	}


	if (strcmp(command[0], "/start") == 0)
	{
		CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_START_SERVER,
			(void*)"26105", 6,
			nullptr,
			nullptr);

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return network;
	}

	if (strcmp(command[0], "/lstart") == 0)
	{
		CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_LOCAL_START_SERVER,
			nullptr, 0,
			nullptr,
			nullptr);

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return network;
	}

	if (strcmp(command[0], "/connect") == 0)
	{
		CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_CONNECT,
			(void*)"127.0.0.1:26105", 16,
			nullptr,
			nullptr);

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return network;
	}

	if (strcmp(command[0], "/lconnect") == 0)
	{
		CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_LOCAL_CONNECT,
			nullptr, 0,
			nullptr,
			nullptr);

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return network;
	}

	if (strcmp(command[0], "/load") == 0)
	{
		CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_LOAD_ENVIRONMENT,
			(void*)"terrain\\vertices.txt", 22,
			(void*)serverInfo->m_playerInfo,
			(void*)serverInfo);

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return network;
	}

	if (strcmp(command[0], "/lload") == 0)
	{
		CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_LOCAL_LOAD_ENVIRONMENT,
			(void*)"terrain\\vertices.txt", 22,
			(void*)serverInfo->m_playerInfo,
			(void*)serverInfo);

		CConsole::AddEventMessage("%s", m_commandLine->GetText());

		return network;
	}

	if (strcmp(command[0], "/addbot") == 0)
	{
		if (strlen(command[1]) > 0)
		{
			CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_ADD_BOT,
				(void*)command[1], (int)strlen(command[1]),
				(void*)serverInfo->m_playerInfo,
				(void*)serverInfo);

			CConsole::AddEventMessage("%s", m_commandLine->GetText());

			return network;
		}

		CConsole::AddEventMessage("addbot with a name");

		return nullptr;
	}

	CConsole::AddEventMessage("Unknown Command %s", command);

	return nullptr;
}

/*
*/
void CConsole::DisplayConsole()
{
	m_background->Draw();
}

/*
*/
void CConsole::DisplayText()
{
	m_lines = (int)((m_bottomRight.p.y - m_topLeft.p.y) / m_font->m_height) - 1;

	m_start = (int)(m_bottomRight.p.y - m_font->m_height - (m_font->m_height * m_entries->m_count)) - 4;

	if (m_start < m_topLeft.p.y)
	{
		m_start = (int)(m_bottomRight.p.y - m_font->m_height - (m_font->m_height * m_lines));
	}


	CLinkListNode<CString>* entries = m_entries->m_list;

	if (m_entries->m_count > m_lines)
	{
		entries = m_entries->Search(m_entries->m_count - m_lines + m_scroll);
	}
	else
	{
		entries = m_entries->m_list;
	}

	while (entries->m_object)
	{
		CVertex2 position(m_topLeft.p.x + 4, (float)m_start);

		m_font->Draw(entries->m_object->GetWText(), position, m_size, m_globalObjects->m_color->SunYellow);

		m_start += (int)m_font->m_height;

		if (m_start > (m_bottomRight.p.y - m_font->m_height - m_font->m_height))
		{
			break;
		}

		entries = entries->m_next;
	}


	CVertex2 position(m_topLeft.p.x + 4, m_bottomRight.p.y - m_font->m_height - 4);

	m_font->Draw(m_commandLine->GetWText(), position, m_size, m_globalObjects->m_color->SunYellow);
}

/*
*/
void CConsole::Paging(char direction)
{
	if (direction == CConsole::E_PAGE_BACKWARD)
	{
		m_scroll--;
	}

	if ((m_entries->m_count - m_lines + m_scroll) < 0)
	{
		m_scroll++;
	}

	if (direction == CConsole::E_PAGE_FORWARD)
	{
		m_scroll++;
	}

	if (m_scroll > 0)
	{
		m_scroll = 0;
	}
}

/*
*/
void CConsole::Reset()
{
	m_cursorLocation = 0;

	m_commandLine->Clear();
}

/*
*/
void CConsole::ToggleVisibility()
{
	m_isVisible = !m_isVisible;
}
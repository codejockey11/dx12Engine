#include "CServerInfo.h"

/*
*/
CServerInfo::CServerInfo()
{
	memset(this, 0x00, sizeof(CServerInfo));
}

/*
*/
CServerInfo::CServerInfo(CServerInfo* serverInfo)
{
	memset(this, 0x00, sizeof(CServerInfo));

	if (serverInfo == nullptr)
	{
		return;
	}

	m_server = serverInfo->m_server;

	m_socket = serverInfo->m_socket;

	m_isRunning = serverInfo->m_isRunning;

	m_hThread = serverInfo->m_hThread;

	m_threadId = serverInfo->m_threadId;


	// This is used to store player info on either the server or the client.
	// Copying it will result in addressing issues between the server or the client.
	// Don't copy it however delete them in either the server or the event manager
	// deconstructor where/when the reference pointers were created.
	//m_playerInfo = serverInfo->m_playerInfo;

	m_botCamera = serverInfo->m_botCamera;

	m_idleTime = serverInfo->m_idleTime;

	m_velocity = serverInfo->m_velocity;

	m_isBot = serverInfo->m_isBot;

	m_timerReload = serverInfo->m_timerReload;
}

/*
*/
CServerInfo::~CServerInfo()
{
}

/*
*/
void CServerInfo::Initialize(CServerInfo* serverInfo)
{
	if (serverInfo == nullptr)
	{
		return;
	}

	m_server = serverInfo->m_server;

	m_socket = serverInfo->m_socket;

	m_isRunning = serverInfo->m_isRunning;

	m_hThread = serverInfo->m_hThread;

	m_threadId = serverInfo->m_threadId;

	// This is used to store player info on either the server or the client.
	// Copying it will result in addressing issues between the server or the client.
	// Don't copy it however delete them in either the server or the event manager
	// deconstructor where/when the reference pointers were created.
	//m_playerInfo = serverInfo->m_playerInfo;

	m_botCamera = serverInfo->m_botCamera;

	m_idleTime = serverInfo->m_idleTime;

	m_velocity = serverInfo->m_velocity;

	m_isFalling = serverInfo->m_isFalling;

	m_isBot = serverInfo->m_isBot;

	m_timerReload = serverInfo->m_timerReload;
}
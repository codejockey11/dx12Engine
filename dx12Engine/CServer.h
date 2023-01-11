#pragma once

#include "standard.h"

#include "CEvent.h"
#include "CClientSocket.h"
#include "CFrametime.h"
#include "CGlobalObjects.h"
#include "CPlayerInfo.h"

class CServer
{
public:

	enum
	{
		MAX_PLAYERS = 5
	};

	CGlobalObjects* m_globalObjects;

	SOCKET m_listenSocket;

	CPlayerInfo* m_redTeamInfo[CServer::MAX_PLAYERS];
	CPlayerInfo* m_blueTeamInfo[CServer::MAX_PLAYERS];

	CClientSocket* m_redClientSocket[CServer::MAX_PLAYERS];
	CClientSocket* m_blueClientSocket[CServer::MAX_PLAYERS];

	int m_client;

	bool m_acceptRunning;

	bool m_serverWorkerRunning;

	CFrametime* m_frametime;

	CServerEnvironment* m_serverEnvironment;

	void (*pFunc[CEvent::CServerEvent::SE_MAX]) (CServer* server, CEvent* evt, CPlayerInfo* playerInfo);


	CServer();

	CServer(CGlobalObjects* globalObjects, const char* port);

	~CServer();


	void InitializeWinsock();

	void CreateListenSocket(const char* port);

	void BindListenSocket(addrinfo* result);

	void StartListenSocket();

	void SendNetwork(CEvent* evt, CClientSocket* clientSocket);

	void Shutdown();

	void ProcessEvent(CEvent* evt);

private:

	HANDLE m_hThread[2];

	UINT m_listenThreadId;

	UINT m_workerThreadId;
};
#include "CServer.h"

unsigned __stdcall ServerAcceptThread(void* obj);
unsigned __stdcall ServerWorkerThread(void* obj);

void CServer_ClientReturn(CServer* server, CEvent* evt, CPlayerInfo* playerInfo);
void CServer_Disconnect(CServer* server, CEvent* evt, CPlayerInfo* playerInfo);
void CServer_LoadEnvironment(CServer* server, CEvent* evt, CPlayerInfo* playerInfo);
void CServer_PlayerMove(CServer* server, CEvent* evt, CPlayerInfo* playerInfo);
void CServer_PlayerReady(CServer* server, CEvent* evt, CPlayerInfo* playerInfo);
void CServer_UpdatePlayerInfo(CServer* server, CEvent* evt, CPlayerInfo* playerInfo);

CServer::CServer()
{
	memset(this, 0x00, sizeof(CServer));
}

CServer::CServer(CGlobalObjects* globalObjects, const char* port)
{
	memset(this, 0x00, sizeof(CServer));

	m_globalObjects = globalObjects;

	m_frametime = new CFrametime();

	pFunc[CEvent::CServerEvent::SE_IDLEPLAYER] = &CServer_ClientReturn;
	pFunc[CEvent::CServerEvent::SE_DISCONNECT] = &CServer_Disconnect;
	pFunc[CEvent::CServerEvent::SE_LOADENVIRONMENT] = &CServer_LoadEnvironment;
	pFunc[CEvent::CServerEvent::SE_PLAYERMOVE] = &CServer_PlayerMove;
	pFunc[CEvent::CServerEvent::SE_PLAYERREADY] = &CServer_PlayerReady;
	pFunc[CEvent::CServerEvent::SE_UPDATEPLAYERINFO] = &CServer_UpdatePlayerInfo;

	CServer::InitializeWinsock();
	CServer::CreateListenSocket(port);
	CServer::StartListenSocket();
}

CServer::~CServer()
{
	if (m_serverEnvironment)
	{
		delete m_serverEnvironment;
	}

	for (int i = 0; i < CServer::MAX_PLAYERS; i++)
	{
		delete m_redTeamInfo[i];
		delete m_redClientSocket[i];

		delete m_blueTeamInfo[i];
		delete m_blueClientSocket[i];
	}

	delete m_frametime;
}

void CServer::InitializeWinsock()
{
	WORD wVersionRequested = MAKEWORD(2, 0);

	WSADATA wsaData;

	int err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::InitializeWinsock::WSAStartup:%i\n", err);

		return;
	}

	m_globalObjects->m_errorLog->WriteError("CServer::InitializeWinsock::WSAStartup:%s\n", wsaData.szDescription);
}

void CServer::CreateListenSocket(const char* port)
{
	struct addrinfo* result = {};
	struct addrinfo* ptr = {};
	struct addrinfo  hints = {};

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int err = getaddrinfo(NULL, port, &hints, &result);

	if (err != 0)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::CreateListenSocket::getaddrinfo:%i\n", err);

		return;
	}

	m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (m_listenSocket == INVALID_SOCKET)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::CreateListenSocket::socket:%s\n", m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());

		freeaddrinfo(result);

		return;
	}

	CServer::BindListenSocket(result);
}

void CServer::BindListenSocket(addrinfo* result)
{
	int err = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (err == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::BindListenSocket::bind:%s\n", m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());

		freeaddrinfo(result);

		closesocket(m_listenSocket);

		m_listenSocket = 0;
	}

	freeaddrinfo(result);
}

void CServer::StartListenSocket()
{
	if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::StartListenSocket::listen:%s\n", m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());

		closesocket(m_listenSocket);

		return;
	}

	m_acceptRunning = true;
	m_serverWorkerRunning = true;

	m_hThread[0] = (HANDLE)_beginthreadex(NULL, 0, &ServerAcceptThread, (void*)this, 0, &m_listenThreadId);
	m_hThread[1] = (HANDLE)_beginthreadex(NULL, 0, &ServerWorkerThread, (void*)this, 0, &m_workerThreadId);
}

void CServer::SendNetwork(CEvent* evt, CClientSocket* cs)
{
	if (!m_acceptRunning)
	{
		return;
	}

	int totalBytes = send(cs->m_socket, (char*)evt, sizeof(CEvent), 0);

	if (totalBytes == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::SendNetwork::send:%s\n", m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());
	}
}

void CServer::Shutdown()
{
	if (!m_acceptRunning)
	{
		return;
	}

	m_globalObjects->m_errorLog->WriteError("CServer::Shutdown\n");


	m_acceptRunning = false;

	m_serverWorkerRunning = false;


	WaitForSingleObject(m_hThread[0], 3000);

	CloseHandle(m_hThread[0]);


	WaitForSingleObject(m_hThread[1], 3000);

	CloseHandle(m_hThread[1]);


	if (m_redClientSocket[0] != nullptr)
	{
		int err = shutdown(m_redClientSocket[0]->m_socket, SD_BOTH);

		if (err == SOCKET_ERROR)
		{
			m_globalObjects->m_errorLog->WriteError("CServer::Shutdown::shutdown:%s\n", m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());
		}

		closesocket(m_redClientSocket[0]->m_socket);
	}


	closesocket(m_listenSocket);

	WSACleanup();
}

void CServer::ProcessEvent(CEvent* evt)
{
	pFunc[evt->m_type](this, evt, (CPlayerInfo*)evt->m_playerInfo);
}

void CServer_ClientReturn(CServer* server, CEvent* evt, CPlayerInfo* playerInfo)
{
}

void CServer_Disconnect(CServer* server, CEvent* evt, CPlayerInfo* playerInfo)
{
	server->m_globalObjects->m_errorLog->WriteError("CServer::ProcessEvent::Client Closing Connection:%i\n", server->m_redClientSocket[playerInfo->m_number]->m_clientNbr);

	
	closesocket(server->m_redClientSocket[playerInfo->m_number]->m_socket);

	
	delete server->m_redClientSocket[playerInfo->m_number];

	server->m_redClientSocket[playerInfo->m_number] = nullptr;

	delete server->m_redTeamInfo[playerInfo->m_number];

	server->m_redTeamInfo[playerInfo->m_number] = nullptr;
}

void CServer_LoadEnvironment(CServer* server, CEvent* evt, CPlayerInfo* playerInfo)
{
	if (server->m_serverEnvironment)
	{
		delete server->m_serverEnvironment;
	}

	server->m_serverEnvironment = new CServerEnvironment((char*)evt->m_data);

	memcpy((void*)server->m_redTeamInfo[playerInfo->m_number], (void*)playerInfo, sizeof(CPlayerInfo));

	server->m_redTeamInfo[playerInfo->m_number]->m_environment = server->m_serverEnvironment;

	CEvent* clientEvent = new CEvent(-1, server->m_redClientSocket[playerInfo->m_number]->m_clientNbr, CEvent::ET_CLIENT, CEvent::ET_LOADENVIRONMENT, (char*)evt->m_data, (int)strlen((char*)evt->m_data), (void*)server->m_redTeamInfo[playerInfo->m_number]);

	server->SendNetwork(clientEvent, server->m_redClientSocket[playerInfo->m_number]);

	delete clientEvent;
}

void CServer_PlayerReady(CServer* server, CEvent* evt, CPlayerInfo* playerInfo)
{
	memcpy((void*)server->m_redTeamInfo[playerInfo->m_number], (void*)playerInfo, sizeof(CPlayerInfo));

	server->m_redTeamInfo[playerInfo->m_number]->m_position = server->m_serverEnvironment->m_redTeamStart[0]->m_position;
	server->m_redTeamInfo[playerInfo->m_number]->m_direction = server->m_serverEnvironment->m_redTeamStart[0]->m_direction;

	server->m_redTeamInfo[playerInfo->m_number]->m_isReady = true;

	CEvent* clientEvent = new CEvent(-1, server->m_redClientSocket[playerInfo->m_number]->m_clientNbr, CEvent::ET_CLIENT, CEvent::ET_PLAYERENTER, nullptr, 0, (void*)server->m_redTeamInfo[playerInfo->m_number]);

	server->SendNetwork(clientEvent, server->m_redClientSocket[playerInfo->m_number]);

	delete clientEvent;
}

void CServer_PlayerMove(CServer* server, CEvent* evt, CPlayerInfo* playerInfo)
{
	memcpy((void*)server->m_redTeamInfo[playerInfo->m_number], (void*)playerInfo, sizeof(CPlayerInfo));

	server->m_redTeamInfo[playerInfo->m_number]->m_state = evt->m_data[0];

	server->m_redTeamInfo[playerInfo->m_number]->PlayerMove();

	CEvent* clientEvent = new CEvent(-1, server->m_redClientSocket[playerInfo->m_number]->m_clientNbr, CEvent::ET_CLIENT, CEvent::ET_PLAYERUPDATE, nullptr, 0, (void*)server->m_redTeamInfo[playerInfo->m_number]);

	server->SendNetwork(clientEvent, server->m_redClientSocket[playerInfo->m_number]);

	delete clientEvent;
}

void CServer_UpdatePlayerInfo(CServer* server, CEvent* evt, CPlayerInfo* playerInfo)
{
	memcpy((void*)server->m_redTeamInfo[playerInfo->m_number], (void*)playerInfo, sizeof(CPlayerInfo));
}

unsigned __stdcall ServerAcceptThread(void* obj)
{
	CServer* server = (CServer*)obj;

#ifdef _DEBUG
	server->m_globalObjects->m_errorLog->WriteError("CServer::ServerAcceptThread Starting\n");
#endif

	while (server->m_acceptRunning)
	{
		if (server->m_listenSocket)
		{
			SOCKET tempSocket = accept(server->m_listenSocket, NULL, NULL);

			// WSAEINTR for some reason the hardware stopped the function call so skipping the error check 
			if ((tempSocket == INVALID_SOCKET) && (WSAGetLastError() != WSAEINTR))
			{
#ifdef _DEBUG
				server->m_globalObjects->m_errorLog->WriteError("CServer::ServerAcceptThread::accept:INVALID_SOCKET:%s\n", server->m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());
#endif
				closesocket(tempSocket);

				tempSocket = 0;
			}
			else if ((tempSocket == SOCKET_ERROR) && (WSAGetLastError() != WSAEINTR))
			{
				server->m_globalObjects->m_errorLog->WriteError("CServer::ServerAcceptThread::accept:SOCKET_ERROR:%s\n", server->m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());
			}
			else
			{
				for (int i = 0; i < CServer::MAX_PLAYERS; i++)
				{
					if (server->m_redClientSocket[i] == nullptr)
					{
						server->m_redClientSocket[i] = new CClientSocket(server->m_client, tempSocket);

						server->m_redTeamInfo[i] = new CPlayerInfo();

						server->m_redTeamInfo[i]->m_frametime = server->m_frametime;

						server->m_redTeamInfo[i]->m_number = i;
						server->m_redTeamInfo[i]->m_team = CEvent::CServerEvent::SE_TEAM_RED;

						CEvent* clientEvent = new CEvent(-1, server->m_redClientSocket[i]->m_clientNbr, CEvent::ET_CLIENT, CEvent::ET_ACCEPTED, nullptr, 0, (void*)server->m_redTeamInfo[i]);

						server->SendNetwork(clientEvent, server->m_redClientSocket[i]);

						delete clientEvent;

						break;
					}
				}
			}
		}
	}

#ifdef _DEBUG
	server->m_globalObjects->m_errorLog->WriteError("CServer::ServerAcceptThread Ending\n");
#endif

	_endthreadex(0);

	return 0;
}

unsigned __stdcall ServerWorkerThread(void* obj)
{
	CServer* server = (CServer*)obj;

#ifdef _DEBUG
	server->m_globalObjects->m_errorLog->WriteError("CServer::ServerWorkerThread Starting\n");
#endif

	while (server->m_serverWorkerRunning)
	{
		server->m_frametime->Frame();

		if (server->m_redClientSocket[0] != nullptr)
		{
			CEvent clientEvent;

			int totalBytes = recv(server->m_redClientSocket[0]->m_socket, (char*)&clientEvent, sizeof(CEvent), 0);

			if (totalBytes > 0)
			{
				server->ProcessEvent(&clientEvent);
			}
			else if (totalBytes == 0)
			{
#ifdef _DEBUG
				server->m_globalObjects->m_errorLog->WriteError("CServer::ServerWorkerThread::Client Closing Connection:%i\n", server->m_redClientSocket[0]->m_clientNbr);
#endif
				closesocket(server->m_redClientSocket[0]->m_socket);

				delete server->m_redClientSocket[0];

				server->m_redClientSocket[0] = nullptr;
			}
			// WSAEINTR for some reason the hardware stopped the function call so skipping the error check 
			else if ((totalBytes == SOCKET_ERROR) && (WSAGetLastError() != WSAEINTR))
			{
				server->m_globalObjects->m_errorLog->WriteError("CServer::ServerWorkerThread::recv:%s\n", server->m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());
			}
		}
	}

#ifdef _DEBUG
	server->m_globalObjects->m_errorLog->WriteError("CServer::ServerWorkerThread Ending\n");
#endif
	_endthreadex(0);

	return 0;
}
#include "CServer.h"

unsigned int __stdcall CServer_AccountThread(void* obj);
unsigned int __stdcall CServer_ListenThread(void* obj);
unsigned int __stdcall CServer_PlayerThread(void* obj);

void ProcessMETARNodes(CServer* server, CServerInfo* serverInfo, CComPtr<IXMLDOMNodeList> nodeList);
void ProcessMETARNode(CServer* server, CServerInfo* serverInfo, CComPtr<IXMLDOMNodeList> nodeList);

void CServer_AddBot(CServer* server, CNetwork* network, CServerInfo* serverInfo);
void CServer_Chat(CServer* server, CNetwork* network, CServerInfo* serverInfo);
void CServer_Disconnect(CServer* server, CNetwork* network, CServerInfo* serverInfo);
void CServer_LoadEnvironment(CServer* server, CNetwork* network, CServerInfo* serverInfo);
void CServer_PlayerIdle(CServer* server, CNetwork* network, CServerInfo* serverInfo);
void CServer_PlayerMove(CServer* server, CNetwork* network, CServerInfo* serverInfo);
void CServer_PlayerReady(CServer* server, CNetwork* network, CServerInfo* serverInfo);
void CServer_SendServerInfo(CServer* server, CNetwork* network, CServerInfo* serverInfo);

/*
*/
CServer::CServer()
{
	memset(this, 0x00, sizeof(CServer));
}

/*
*/
CServer::CServer(CGlobalObjects* globalObjects)
{
	memset(this, 0x00, sizeof(CServer));

	m_globalObjects = globalObjects;

	m_network = new CNetwork();

	m_collision = new CCollision();

	m_serverInfo = new CLinkList<CServerInfo>();


	pFunc[CNetwork::ServerEvent::E_SE_ADD_BOT] = &CServer_AddBot;
	pFunc[CNetwork::ServerEvent::E_SE_CHAT] = &CServer_Chat;
	pFunc[CNetwork::ServerEvent::E_SE_DISCONNECT] = &CServer_Disconnect;
	pFunc[CNetwork::ServerEvent::E_SE_LOAD_ENVIRONMENT] = &CServer_LoadEnvironment;
	pFunc[CNetwork::ServerEvent::E_SE_PLAYER_IDLE] = &CServer_PlayerIdle;
	pFunc[CNetwork::ServerEvent::E_SE_PLAYER_MOVE] = &CServer_PlayerMove;
	pFunc[CNetwork::ServerEvent::E_SE_PLAYER_READY] = &CServer_PlayerReady;
	pFunc[CNetwork::ServerEvent::E_SE_SEND_SERVER_INFO] = &CServer_SendServerInfo;
}

/*
*/
CServer::~CServer()
{
	CServer::Shutdown();

	CLinkListNode<CServerInfo>* lln = m_serverInfo->m_list;

	while (lln->m_object)
	{
		delete lln->m_object->m_timerReload;
		delete lln->m_object->m_playerInfo;

		lln = lln->m_next;
	}

	delete m_serverInfo;

	if (m_serverEnvironment)
	{
		delete m_serverEnvironment;
	}

	delete m_collision;

	delete m_network;
}

/*
*/
void CServer::Accept()
{
	SOCKET tempSocket = accept(m_listenSocket, NULL, NULL);

	if (tempSocket == SOCKET_ERROR)
	{
#ifdef _DEBUG
		m_globalObjects->m_errorLog->WriteError("CServer_ListenThread::accept:SOCKET_ERROR:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
#endif
		m_listenThreadRunning = false;
	}
	else if (tempSocket == INVALID_SOCKET)
	{
#ifdef _DEBUG
		m_globalObjects->m_errorLog->WriteError("CServer_ListenThread::accept:INVALID_SOCKET:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
#endif

		m_listenThreadRunning = false;
	}
	else
	{
		CServer::CreatePlayer(nullptr, false, tempSocket);
	}
}

/*
*/
void CServer::BotInference(CServerInfo* serverInfo, CFrametime* frametime)
{
	if (!m_localServerRunning)
	{
		// 20 frames per second
		if (frametime->m_counter >= 20)
		{
			// potential 50 millisecond sleep
			ULONGLONG ms = 30;
			long sleeptime = (long)(ms - frametime->m_totalTime);

			if (sleeptime > 0)
			{
				Sleep(sleeptime);
			}

			frametime->m_counter = 0;
			frametime->m_totalTime = 0;
		}
	}

	if (m_localServerRunning)
	{
		serverInfo->m_playerInfo->m_camera->UpdateRotation(0.0f, 50.0f * frametime->m_frametime, 0.0f);

		serverInfo->m_playerInfo->m_camera->UpdateView();

		serverInfo->m_playerInfo->SetDirection(&serverInfo->m_playerInfo->m_camera->m_look);
	}
	else
	{
		serverInfo->m_botCamera->UpdateRotation(0.0f, 50.0f * frametime->m_frametime, 0.0f);

		serverInfo->m_botCamera->UpdateView();

		serverInfo->m_playerInfo->SetDirection(&serverInfo->m_botCamera->m_look);
	}

	BYTE direction = CNetwork::E_CE_PLAYER_FORWARD;

	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_MOVE,
		(void*)&direction, 1,
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	CServer::ProcessEvent(n, frametime);

	delete n;
}

/*
*/
void CServer::CreateListenSocket(const char* port)
{
	struct addrinfo* addrResult = {};
	struct addrinfo* addrPtr = {};
	struct addrinfo  addrHints = {};

	addrHints.ai_family = AF_INET;
	addrHints.ai_socktype = SOCK_STREAM;
	addrHints.ai_protocol = IPPROTO_TCP;
	addrHints.ai_flags = AI_PASSIVE;

	int result = getaddrinfo(NULL, port, &addrHints, &addrResult);

	if (result != 0)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::CreateListenSocket::getaddrinfo:%i\n", result);

		return;
	}

	m_listenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);

	if (m_listenSocket == INVALID_SOCKET)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::CreateListenSocket::socket:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());

		freeaddrinfo(addrResult);

		return;
	}

	result = bind(m_listenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);

	if (result == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::BindListenSocket::bind:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
	}

	freeaddrinfo(addrResult);
}

/*
*/
void CServer::CreatePlayer(CNetwork* network, bool isBot, SOCKET tempSocket)
{
	CServerInfo* serverInfo = new CServerInfo();

	serverInfo->m_timerReload = new CTimer(500);

	serverInfo->m_server = this;

	serverInfo->m_isBot = isBot;

	serverInfo->m_socket = tempSocket;

	serverInfo->m_isRunning = true;


	serverInfo->m_playerInfo = new CPlayerInfo();


	if (isBot)
	{
		serverInfo->m_botCamera = new CCamera((float)m_globalObjects->m_window->m_width,
			(float)m_globalObjects->m_window->m_height,
			&serverInfo->m_playerInfo->m_position,
			45.0f,
			1.0f, 50000.0f);
	}


	if (m_redTeamCount > m_blueTeamCount)
	{
		serverInfo->m_playerInfo->m_team = CPlayerInfo::E_TEAM_BLUE;
		m_blueTeamCount++;
	}
	else
	{
		serverInfo->m_playerInfo->m_team = CPlayerInfo::E_TEAM_RED;
		m_redTeamCount++;
	}

	if (isBot)
	{
		serverInfo->m_playerInfo->SetName((char*)network->m_data);
	}
	else
	{
		serverInfo->m_playerInfo->SetName((char*)"Steve");
	}


	m_serverInfo->Add(serverInfo, serverInfo->m_playerInfo->m_name);

	// If a non-networked server is running skip the threads
	if (m_localServerRunning)
	{
		return;
	}


	if (isBot)
	{
		serverInfo->m_hThread = (HANDLE)_beginthreadex(
			NULL,
			0,
			&CServer_PlayerThread,
			(void*)serverInfo,
			0,
			&serverInfo->m_threadId);

		CServer_PlayerReady(this, network, serverInfo);
	}
	else
	{
		serverInfo->m_hThread = (HANDLE)_beginthreadex(
			NULL,
			0,
			&CServer_PlayerThread,
			(void*)serverInfo,
			0,
			&serverInfo->m_threadId);

		CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_ACCEPTED,
			nullptr, 0,
			(void*)serverInfo->m_playerInfo,
			(void*)serverInfo);

		CServer::SendNetwork(n, serverInfo->m_socket);

		delete n;

		CAccountInfo* accountInfo = new CAccountInfo(m_globalObjects, this, serverInfo, nullptr);

		HANDLE handle = (HANDLE)_beginthreadex(
			NULL,
			0,
			&CServer_AccountThread,
			(void*)accountInfo,
			0,
			nullptr);


		if (m_serverEnvironment != nullptr)
		{
			n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_LOAD_ENVIRONMENT,
				(void*)m_serverEnvironment->m_name->GetText(), m_serverEnvironment->m_name->GetLength(),
				(void*)serverInfo->m_playerInfo,
				(void*)serverInfo);

			CServer::SendNetwork(n, serverInfo->m_socket);

			delete n;
		}
	}
}

/*
*/
void CServer::InitializeWinsock()
{
	WORD wVersionRequested = MAKEWORD(2, 0);

	int result = WSAStartup(wVersionRequested, &wsaData);

	if (result != 0)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::InitializeWinsock::WSAStartup:%i\n", result);

		return;
	}

	m_globalObjects->m_errorLog->WriteError("CServer::InitializeWinsock::WSAStartup:%s\n", wsaData.szDescription);
}

/*
*/
void CServer::ProcessEvent(CNetwork* network, CFrametime* frametime)
{
	m_collision->m_frametime = frametime;

	if (!m_localServerRunning)
	{
		m_frametime = frametime;
	}

	CServerInfo* serverInfo = {};

	CPlayerInfo* playerInfo = (CPlayerInfo*)network->m_playerInfo;

	CLinkListNode<CServerInfo>* lln = m_serverInfo->Search(playerInfo->m_name);


	if (lln == nullptr)
	{
		CServerInfo* si = new CServerInfo((CServerInfo*)network->m_serverInfo);

		si->m_playerInfo = new CPlayerInfo();

		si->m_playerInfo->Initialize(playerInfo);

		si->m_timerReload = new CTimer(500);

		si->m_server = this;

		m_serverInfo->Add(si, si->m_playerInfo->m_name);

		serverInfo = si;
	}
	else
	{
		serverInfo = lln->m_object;

		serverInfo->m_playerInfo->Initialize(playerInfo);
	}


	pFunc[network->m_type](this, network, serverInfo);
}

/*
*/
void CServer::ReceivePlayer(CServerInfo* serverInfo, CFrametime* frametime)
{
	int totalBytes = recv(serverInfo->m_socket, (char*)m_network, sizeof(CNetwork), 0);

	if (totalBytes > 0)
	{
		CServer::ProcessEvent(m_network, frametime);
	}
	else if (totalBytes == 0)
	{
#ifdef _DEBUG
		m_globalObjects->m_errorLog->WriteError("CServer_ClientThread::Client Closing Connection:%s\n",
			serverInfo->m_playerInfo->m_name);
#endif
	}
	else if (totalBytes == INVALID_SOCKET)
	{
#ifdef _DEBUG
		m_globalObjects->m_errorLog->WriteError("CServer_ClientThread::INVALID_SOCKET:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
#endif
	}
	else if (totalBytes == SOCKET_ERROR)
	{
#ifdef _DEBUG
		m_globalObjects->m_errorLog->WriteError("CServer_ClientThread::SOCKET_ERROR:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
#endif
	}
}

/*
*/
void CServer::SendNetwork(CNetwork* network)
{
	if (m_localServerRunning)
	{
		return;
	}

	CLinkListNode<CServerInfo>* lln = m_serverInfo->m_list;

	while (lln->m_object)
	{
		if (lln->m_object->m_socket != 0)
		{
			int totalBytes = send(lln->m_object->m_socket, (char*)network, sizeof(CNetwork), 0);

			if (totalBytes == SOCKET_ERROR)
			{
#ifdef _DEBUG
				m_globalObjects->m_errorLog->WriteError("CServer::SendNetwork(All)::send:%s %s\n",
					m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
					m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
#endif
				lln->m_object->m_socket = 0;
			}
		}

		lln = lln->m_next;
	}
}

/*
*/
void CServer::SendNetwork(CNetwork* network, SOCKET tempSocket)
{
	if (m_localServerRunning)
	{
		return;
	}

	int totalBytes = send(tempSocket, (char*)network, sizeof(CNetwork), 0);

	if (totalBytes == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::SendNetwork::send:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
	}
}

/*
*/
void CServer::Shutdown()
{
	if (m_localServerRunning)
	{
		CServer::ShutdownLocal();
	}

	if (m_listenThreadRunning)
	{
		CServer::ShutdownListen();

		CServer::ShutdownPlayers();
	}

	WSACleanup();
}

/*
*/
void CServer::ShutdownListen()
{
	m_listenThreadRunning = false;

	// This shutdown will end the thread with WSAENOTCONN and WSAEINTR being acceptable
	int result = shutdown(m_listenSocket, SD_BOTH);


	// Thread is hanging on the accept so wait for it to finish.
	// The informational messages can be ignored WSAENOTCONN because no one is connected and
	// WSAEINTR because the accept socket was interupted with the shutdown function.
	WaitForSingleObject(m_listenThreadHandle, 50);

	CloseHandle(m_listenThreadHandle);


#ifdef _DEBUG
	if (result != 0)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::Shutdown::shutdown:m_listenSocket:SOCKET_ERROR:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
	}
#endif
}

/*
*/
void CServer::ShutdownLocal()
{
	m_localServerRunning = false;

	delete m_frametime;
}

/*
*/
void CServer::ShutdownPlayers()
{
	CLinkListNode<CServerInfo>* lln = m_serverInfo->m_list;

	while (lln->m_object)
	{
		lln->m_object->m_isRunning = false;

		// Wait on the bot thread
		if (lln->m_object->m_isBot)
		{
			WaitForSingleObject(lln->m_object->m_hThread, 50);

			CloseHandle(lln->m_object->m_hThread);
		}

		if (lln->m_object->m_socket != 0)
		{
			int result = shutdown(lln->m_object->m_socket, SD_BOTH);

			// Same as accept socket
			WaitForSingleObject(lln->m_object->m_hThread, 50);

			CloseHandle(lln->m_object->m_hThread);


			if (result != 0)
			{
#ifdef _DEBUG
				m_globalObjects->m_errorLog->WriteError("CServer::ShutdownPlayers:m_socket:%s %s\n",
					m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
					m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
#endif
			}
		}

		lln = lln->m_next;
	}
}

/*
*/
void CServer::StartServer()
{
	m_frametime = new CFrametime();

	m_localServerRunning = true;
}

/*
*/
void CServer::StartServer(const char* port)
{
	CServer::InitializeWinsock();
	CServer::CreateListenSocket(port);
	CServer::StartListenSocket();
}

/*
*/
void CServer::StartListenSocket()
{
	if (listen(m_listenSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CServer::StartListenSocket::listen:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());

		return;
	}

	m_listenThreadRunning = true;

	m_listenThreadHandle = (HANDLE)_beginthreadex(NULL, 0, &CServer_ListenThread, (void*)this, 0, &m_listenThreadId);
}

/*
*/
void CServer_AddBot(CServer* server, CNetwork* network, CServerInfo* serverInfo)
{
	server->CreatePlayer(network, true, 0);
}

/*
*/
void CServer_Chat(CServer* server, CNetwork* network, CServerInfo* serverInfo)
{
	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_CHAT,
		(char*)network->m_data, (int)strlen((char*)network->m_data),
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	// Send to all the clients
	server->SendNetwork(n);

	delete n;
}

/*
*/
void CServer_Disconnect(CServer* server, CNetwork* network, CServerInfo* serverInfo)
{
	server->m_globalObjects->m_errorLog->WriteError("CServer_Disconnect::Client Closing Connection:%s\n",
		serverInfo->m_playerInfo->m_name);


	CString* message = new CString(serverInfo->m_playerInfo->m_name);

	message->Concat(" disconnected");

	network->SetData(message->GetText(), message->GetLength());

	server->SendNetwork(network);

	delete message;


	switch (serverInfo->m_playerInfo->m_team)
	{
	case CPlayerInfo::E_TEAM_BLUE:
	{
		server->m_blueTeamCount--;

		break;
	}
	case CPlayerInfo::E_TEAM_RED:
	{
		server->m_redTeamCount--;

		break;
	}
	}


	serverInfo->m_isRunning = false;

	WaitForSingleObject(serverInfo->m_hThread, 50);

	CloseHandle(serverInfo->m_hThread);


	if (serverInfo->m_socket != 0)
	{
		int result = shutdown(serverInfo->m_socket, SD_BOTH);

		serverInfo->m_socket = 0;

		if (result != 0)
		{
			server->m_globalObjects->m_errorLog->WriteError("CServer_Disconnect::shutdown:m_socket:%s %s\n",
				server->m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
				server->m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
		}
	}
}

/*
*/
void CServer_LoadEnvironment(CServer* server, CNetwork* network, CServerInfo* serverInfo)
{
	if (server->m_serverEnvironment)
	{
		delete server->m_serverEnvironment;
	}

	server->m_serverEnvironment = new CServerEnvironment(server->m_globalObjects, (char*)network->m_data);

	server->m_collision->m_environment = server->m_serverEnvironment;

	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_LOAD_ENVIRONMENT,
		(char*)network->m_data, (int)strlen((char*)network->m_data),
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	// Send to all the clients
	server->SendNetwork(n);

	delete n;
}

/*
*/
void CServer_PlayerIdle(CServer* server, CNetwork* network, CServerInfo* serverInfo)
{
	CServer_PlayerMove(server, network, serverInfo);


	serverInfo->m_idleTime += server->m_frametime->m_frametime;


	if (server->m_localServerRunning)
	{
		return;
	}

	// Client idle state too long
	if (serverInfo->m_idleTime >= 50000.0f)
	{
		CServer_Disconnect(server, network, serverInfo);

		return;
	}

	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_PLAYER_UPDATE,
		nullptr, 0,
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	// Send to all the clients
	server->SendNetwork(n);

	delete n;

}

/*
*/
void CServer_PlayerMove(CServer* server, CNetwork* network, CServerInfo* serverInfo)
{
	serverInfo->m_idleTime = 0.0f;

	serverInfo->m_playerInfo->m_state = network->m_data[0];

	if (serverInfo->m_playerInfo->m_state == CNetwork::ClientEvent::E_CE_PLAYER_ATTACK)
	{
		if (!serverInfo->m_timerReload->m_isReloading)
		{
			serverInfo->m_timerReload->Start();

			CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_PLAYER_ATTACK,
				(char*)network->m_data, (int)strlen((char*)network->m_data),
				(void*)serverInfo->m_playerInfo,
				(void*)serverInfo);

			// Send to all the clients
			server->SendNetwork(n);

			delete n;
		}
		else
		{
			serverInfo->m_playerInfo->m_state = 0;
		}
	}
	else if (serverInfo->m_timerReload != nullptr)
	{
		if (serverInfo->m_timerReload->m_isReloading)
		{
			serverInfo->m_playerInfo->m_state = 0;
		}
	}


	server->m_collision->PlayerMove(serverInfo);



	if (server->m_serverEnvironment == nullptr)
	{
		return;
	}

	// Check if the player collided with a collectable item.
	// Grab the collectable list for the cube where the player is located.
	int px = (int)(serverInfo->m_playerInfo->m_position.p.x + (server->m_serverEnvironment->m_width / 2.0f)) / server->m_serverEnvironment->m_gridUnits;
	int pz = (int)(serverInfo->m_playerInfo->m_position.p.z + (server->m_serverEnvironment->m_height / 2.0f)) / server->m_serverEnvironment->m_gridUnits;

	CLinkList<CObject>* collectables = (CLinkList<CObject>*)server->m_serverEnvironment->m_collectables->GetElement(2, px, pz);

	// Make sure the location is not out-of-range.
	if (collectables != nullptr)
	{
		// Make sure something is in the list.
		if (collectables->m_count > 0)
		{
			CLinkListNode<CObject>* collectable = collectables->m_list;

			while (collectable->m_object)
			{
				if (!collectable->m_object->m_limboTimer->m_isReloading)
				{
					CVertex p = serverInfo->m_playerInfo->m_position - CVertex(collectable->m_object->m_position.x, collectable->m_object->m_position.y, collectable->m_object->m_position.z);

					float l = p.Length();

					if (abs(l) <= 2.0f)
					{
						collectable->m_object->m_limboTimer->Start();

						CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_UPDATE_COLLECTABLE,
							(void*)"item01", 6,
							(void*)serverInfo->m_playerInfo,
							(void*)serverInfo);

						// Send to all the clients
						server->SendNetwork(n);

						delete n;
					}
				}
				else
				{
					collectable->m_object->m_limboTimer->Frame();
				}

				collectable = collectable->m_next;
			}
		}
	}





	if (server->m_localServerRunning)
	{
		return;
	}


	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_PLAYER_UPDATE,
		nullptr, 0,
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	// Send to all the clients
	server->SendNetwork(n);

	delete n;
}

/*
*/
void CServer_PlayerReady(CServer* server, CNetwork* network, CServerInfo* serverInfo)
{
	srand((UINT)time(NULL));

	/* generate secret number: */
	int location = rand() % 4;

	if (serverInfo->m_playerInfo->m_team == CPlayerInfo::E_TEAM_RED)
	{
		serverInfo->m_playerInfo->m_position = server->m_serverEnvironment->m_redTeamStarts->Search(location)->m_object->m_position;
		serverInfo->m_playerInfo->m_direction = server->m_serverEnvironment->m_redTeamStarts->Search(location)->m_object->m_direction;
	}

	if (serverInfo->m_playerInfo->m_team == CPlayerInfo::E_TEAM_BLUE)
	{
		serverInfo->m_playerInfo->m_position = server->m_serverEnvironment->m_blueTeamStarts->Search(location)->m_object->m_position;
		serverInfo->m_playerInfo->m_direction = server->m_serverEnvironment->m_blueTeamStarts->Search(location)->m_object->m_direction;
	}

	serverInfo->m_isFalling = true;


	// There is no connection to the remote clients when a local server is running
	// see comments in CEventManager::Send
	if (server->m_localServerRunning)
	{
		return;
	}

	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_PLAYER_ENTER,
		(void*)serverInfo->m_playerInfo->m_name, (int)strlen(serverInfo->m_playerInfo->m_name),
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	// Send to all the clients
	server->SendNetwork(n);

	delete n;
}

/*
*/
void CServer_SendServerInfo(CServer* server, CNetwork* network, CServerInfo* serverInfo)
{
	// Send all clients to the requester
	CLinkListNode<CServerInfo>* lln = server->m_serverInfo->m_list;

	while (lln->m_object)
	{
		CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_SERVER_INFO,
			nullptr, 0,
			(void*)lln->m_object->m_playerInfo,
			(void*)lln->m_object);

		server->SendNetwork(n, serverInfo->m_socket);

		delete n;

		lln = lln->m_next;
	}
}

/*
* Account retrieve and send to client
*/
unsigned int __stdcall CServer_AccountThread(void* obj)
{
	CAccountInfo* accountInfo = (CAccountInfo*)obj;

	CServer* server = (CServer*)accountInfo->m_server;

	CServerInfo* serverInfo = accountInfo->m_serverInfo;


#ifdef _DEBUG
	server->m_globalObjects->m_errorLog->WriteError("CServer_AccountThread Starting\n");
#endif

	// http request (server would be on same machine so not much need for https) to retrieve the clients account data in an XML format
	// or can be accomplished with MySql and formatting the result rows as required.
	accountInfo->RequestAccountInfo();

	// Concatenating all the buffers that were returned
	CString* buffer = new CString("");

	CLinkListNode<CString>* bufferNode = accountInfo->m_httpRequest->m_buffers->m_list;

	while (bufferNode->m_object)
	{
		buffer->Concat(bufferNode->m_object->GetText());

		bufferNode = bufferNode->m_next;
	}

#ifdef _DEBUG
	server->m_globalObjects->m_errorLog->WriteBytes(buffer->GetText());
#endif

	// loadXML is expecting the string to be encoded utf-16
	CComBSTR arg(buffer->GetWText());

	// done with the buffer
	delete buffer;

	// Begin XML parse
	CComPtr<IXMLDOMDocument> m_iXMLDoc;

	HRESULT hr = m_iXMLDoc.CoCreateInstance(__uuidof(DOMDocument60));

	VARIANT_BOOL bSuccess = {};

	hr = m_iXMLDoc->loadXML(arg, &bSuccess);

	if (hr == S_OK)
	{
		CComPtr<IXMLDOMElement> docRoot;
		CComPtr<IXMLDOMNodeList> nodeList;
		CComPtr<IXMLDOMNode> nodeTemp;

		DOMNodeType nodeType = {};

		hr = m_iXMLDoc->get_documentElement(&docRoot);

		hr = docRoot->get_childNodes(&nodeList);

		BSTR nodeName[128] = {};
		BSTR nodeValue[128] = {};

		hr = nodeList->nextNode(&nodeTemp);

		while (hr == S_OK)
		{
			nodeTemp->get_nodeType(&nodeType);

			switch (nodeType)
			{
			case NODE_ELEMENT:
			{
				nodeTemp->get_nodeName(nodeName);

				nodeTemp->get_text(nodeValue);

				CString* node = new CString(nodeName[0]);

				if (node->Compare("data") == 0)
				{
					CComPtr<IXMLDOMNodeList> metarNodes;

					hr = nodeTemp->get_childNodes(&metarNodes);

					ProcessMETARNodes(server, serverInfo, metarNodes);

					metarNodes.Release();
				}
				else
				{
					CString* value = new CString(nodeValue[0]);

					node->Concat(":");

					node->Concat(value->GetText());

					CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_ACCOUNT_INFO,
						(void*)node->GetText(), node->GetLength(),
						(void*)serverInfo->m_playerInfo,
						(void*)serverInfo);

					server->SendNetwork(network, serverInfo->m_socket);

					delete network;

					delete value;
				}

				delete node;

				break;
			}
			case NODE_ATTRIBUTE:
			{
				break;
			}
			case NODE_TEXT:
			{
				break;
			}
			case NODE_COMMENT:
			{
				break;
			}
			}

			nodeTemp.Release();

			hr = nodeList->nextNode(&nodeTemp);
		}

		nodeTemp.Release();
		nodeList.Release();
		docRoot.Release();
	}

#ifdef _DEBUG
	server->m_globalObjects->m_errorLog->WriteError("CServer_AccountThread Ending\n");
#endif

	_endthreadex(0);

	delete accountInfo;

	return 1;
}

/*
*/
void ProcessMETARNodes(CServer* server, CServerInfo* serverInfo, CComPtr<IXMLDOMNodeList> nodeList)
{
	CComPtr<IXMLDOMNode> nodeTemp;

	DOMNodeType nodeType = {};

	BSTR nodeName[128] = {};
	BSTR nodeValue[128] = {};

	HRESULT hr = nodeList->nextNode(&nodeTemp);

	while (hr == S_OK)
	{
		nodeTemp->get_nodeType(&nodeType);

		switch (nodeType)
		{
		case NODE_ELEMENT:
		{
			nodeTemp->get_nodeName(nodeName);

			nodeTemp->get_text(nodeValue);

			CString* node = new CString(nodeName[0]);

			if (node->Compare("METAR") == 0)
			{
				CComPtr<IXMLDOMNodeList> metarNodes;

				hr = nodeTemp->get_childNodes(&metarNodes);

				ProcessMETARNode(server, serverInfo, metarNodes);

				metarNodes.Release();
			}

			delete node;

			break;
		}
		}

		nodeTemp.Release();

		hr = nodeList->nextNode(&nodeTemp);
	}

	nodeTemp.Release();
}

/*
*/
void ProcessMETARNode(CServer* server, CServerInfo* serverInfo, CComPtr<IXMLDOMNodeList> nodeList)
{
	CComPtr<IXMLDOMNode> nodeTemp;

	DOMNodeType nodeType = {};

	BSTR nodeName[128] = {};
	BSTR nodeValue[128] = {};

	HRESULT hr = nodeList->nextNode(&nodeTemp);

	while (hr == S_OK)
	{
		nodeTemp->get_nodeType(&nodeType);

		switch (nodeType)
		{
		case NODE_ELEMENT:
		{
			nodeTemp->get_nodeName(nodeName);

			nodeTemp->get_text(nodeValue);

			CString* node = new CString(nodeName[0]);

			// child nodes
			if (node->Compare("quality_control_flags") == 0)
			{
				CComPtr<IXMLDOMNodeList> nodeFlagList;
				CComPtr<IXMLDOMNode> nodeFlags;

				nodeTemp->get_childNodes(&nodeFlagList);

				HRESULT hr = nodeFlagList->nextNode(&nodeFlags);

				while (hr == S_OK)
				{
					BSTR flagName[128] = {};
					BSTR flagValue[128] = {};

					nodeFlags->get_nodeName(flagName);
					nodeFlags->get_text(flagValue);

					CString* attrName = new CString(flagName[0]);

					CString* attrValue = new CString(flagValue[0]);

					attrName->Concat(":");

					attrName->Concat(attrValue->GetText());


					CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_ACCOUNT_INFO,
						(void*)attrName->GetText(), attrName->GetLength(),
						(void*)serverInfo->m_playerInfo,
						(void*)serverInfo);

					server->SendNetwork(network, serverInfo->m_socket);

					delete network;

					delete attrValue;

					delete attrName;

					nodeFlags.Release();

					hr = nodeFlagList->nextNode(&nodeFlags);
				}

				nodeFlags.Release();
				nodeFlagList.Release();
			}
			// attribute nodes
			else if (node->Compare("sky_condition") == 0)
			{
				CComPtr<IXMLDOMNamedNodeMap> attributes;
				CComPtr<IXMLDOMNode> nodeAttribute;

				long count = {};

				nodeTemp->get_attributes(&attributes);
				attributes->get_length(&count);

				HRESULT hr = attributes->nextNode(&nodeAttribute);

				while (hr == S_OK)
				{
					BSTR attributeName[128] = {};
					BSTR attributeValue[128] = {};

					nodeAttribute->get_nodeName(attributeName);
					nodeAttribute->get_text(attributeValue);

					CString* attrName = new CString(attributeName[0]);

					CString* attrValue = new CString(attributeValue[0]);

					attrName->Concat(":");

					attrName->Concat(attrValue->GetText());


					CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_ACCOUNT_INFO,
						(void*)attrName->GetText(), attrName->GetLength(),
						(void*)serverInfo->m_playerInfo,
						(void*)serverInfo);

					server->SendNetwork(network, serverInfo->m_socket);

					delete network;

					delete attrValue;

					delete attrName;

					nodeAttribute.Release();

					hr = attributes->nextNode(&nodeAttribute);
				}

				nodeAttribute.Release();
				attributes.Release();
			}
			// element tag and value
			else
			{
				CString* value = new CString(nodeValue[0]);

				node->Concat(":");

				node->Concat(value->GetText());

				CNetwork* network = new CNetwork(CNetwork::ClientEvent::E_CE_TO_CLIENT, CNetwork::ClientEvent::E_CE_ACCOUNT_INFO,
					(void*)node->GetText(), node->GetLength(),
					(void*)serverInfo->m_playerInfo,
					(void*)serverInfo);

				server->SendNetwork(network, serverInfo->m_socket);

				delete network;

				delete value;
			}

			delete node;

			break;
		}
		}

		nodeTemp.Release();

		hr = nodeList->nextNode(&nodeTemp);
	}

	nodeTemp.Release();
}


/*
* Waiting for connection from remote clients
*/
unsigned __stdcall CServer_ListenThread(void* obj)
{
	CServer* server = (CServer*)obj;

#ifdef _DEBUG
	server->m_globalObjects->m_errorLog->WriteError("CServer_ListenThread Starting\n");
#endif

	while (server->m_listenThreadRunning)
	{
		if (server->m_listenSocket)
		{
			server->Accept();
		}
	}

	// No need to close a listen socket after a shutdown

#ifdef _DEBUG
	server->m_globalObjects->m_errorLog->WriteError("CServer_ListenThread Ending\n");
#endif

	_endthreadex(0);

	return 1;
}

/*
*/
unsigned __stdcall CServer_PlayerThread(void* obj)
{
	CServerInfo* serverInfo = (CServerInfo*)obj;

	CServer* server = (CServer*)serverInfo->m_server;

	CFrametime* frametime = new CFrametime();

#ifdef _DEBUG
	server->m_globalObjects->m_errorLog->WriteError("CServer_PlayerThread Starting\n");
#endif

	while (serverInfo->m_isRunning)
	{
		frametime->Frame();

		if (serverInfo->m_timerReload->m_isReloading)
		{
			serverInfo->m_timerReload->Frame();
		}

		if (serverInfo->m_isBot)
		{
			server->BotInference(serverInfo, frametime);
		}
		else
		{
			server->ReceivePlayer(serverInfo, frametime);
		}
	}

	delete frametime;

	frametime = nullptr;

#ifdef _DEBUG
	server->m_globalObjects->m_errorLog->WriteError("CServer_PlayerThread Ending\n");
#endif

	_endthreadex(0);

	return 1;
}
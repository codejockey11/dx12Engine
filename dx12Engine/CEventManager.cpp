#include "CEventManager.h"

// Network thread for client receive events
unsigned __stdcall CEventManager_ReceiveThread(void* obj);

// Event manager functions
void CEventManager_Accepted(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_AccountInfo(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_Chat(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_ClientConnect(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_Disconnect(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_Exit(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_LoadEnvironment(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_LocalConnect(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_LocalLoadEnvironment(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_LocalStartServer(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_PlayerAttack(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_PlayerEnter(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_PlayerExit(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_PlayerInfoRequest(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_PlayerUpdate(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_ServerInfo(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_StartServer(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_StopServer(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_ToggleConsole(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_ToggleWindowmode(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);
void CEventManager_UpdateCollectable(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);

/*
*/
CEventManager::CEventManager()
{
	memset(this, 0x00, sizeof(CEventManager));
}

/*
*/
CEventManager::CEventManager(CGlobalObjects* globalObjects, CConsole* console)
{
	memset(this, 0x00, sizeof(CEventManager));

	m_globalObjects = globalObjects;

	m_console = console;

	m_commandListBundle = new CCommandListBundle(m_globalObjects->m_errorLog, m_globalObjects->m_videoDevice);

	m_serverInfo = new CLinkList<CServerInfo>();

#ifdef _DEBUG
	constexpr auto playerInfoSize = sizeof(CPlayerInfo);
	constexpr auto serverInfoSize = sizeof(CServerInfo);

	m_globalObjects->m_errorLog->WriteError("Sizes:p%u s%u\n", playerInfoSize, serverInfoSize);
#endif

	pFunc[CNetwork::ClientEvent::E_CE_ACCEPTED] = &CEventManager_Accepted;
	pFunc[CNetwork::ClientEvent::E_CE_ACCOUNT_INFO] = &CEventManager_AccountInfo;
	pFunc[CNetwork::ClientEvent::E_CE_CHAT] = &CEventManager_Chat;
	pFunc[CNetwork::ClientEvent::E_CE_CONNECT] = &CEventManager_ClientConnect;
	pFunc[CNetwork::ClientEvent::E_CE_DISCONNECT] = &CEventManager_Disconnect;
	pFunc[CNetwork::ClientEvent::E_CE_EXIT_GAME] = &CEventManager_Exit;
	pFunc[CNetwork::ClientEvent::E_CE_LOAD_ENVIRONMENT] = &CEventManager_LoadEnvironment;
	pFunc[CNetwork::ClientEvent::E_CE_LOCAL_CONNECT] = &CEventManager_LocalConnect;
	pFunc[CNetwork::ClientEvent::E_CE_LOCAL_LOAD_ENVIRONMENT] = &CEventManager_LocalLoadEnvironment;
	pFunc[CNetwork::ClientEvent::E_CE_LOCAL_START_SERVER] = &CEventManager_LocalStartServer;
	pFunc[CNetwork::ClientEvent::E_CE_PLAYER_ATTACK] = &CEventManager_PlayerAttack;
	pFunc[CNetwork::ClientEvent::E_CE_PLAYER_ENTER] = &CEventManager_PlayerEnter;
	pFunc[CNetwork::ClientEvent::E_CE_PLAYER_EXIT] = &CEventManager_PlayerExit;
	pFunc[CNetwork::ClientEvent::E_CE_PLAYER_INFO] = &CEventManager_PlayerInfoRequest;
	pFunc[CNetwork::ClientEvent::E_CE_PLAYER_UPDATE] = &CEventManager_PlayerUpdate;
	pFunc[CNetwork::ClientEvent::E_CE_SERVER_INFO] = &CEventManager_ServerInfo;
	pFunc[CNetwork::ClientEvent::E_CE_START_SERVER] = &CEventManager_StartServer;
	pFunc[CNetwork::ClientEvent::E_CE_STOP_SERVER] = &CEventManager_StopServer;
	pFunc[CNetwork::ClientEvent::E_CE_TOGGLE_CONSOLE] = &CEventManager_ToggleConsole;
	pFunc[CNetwork::ClientEvent::E_CE_TOGGLE_WINDOW] = &CEventManager_ToggleWindowmode;
	pFunc[CNetwork::ClientEvent::E_CE_UPDATE_COLLECTABLE] = &CEventManager_UpdateCollectable;
}

/*
*/
CEventManager::~CEventManager()
{
	// Always shutdown the server first
	if (m_server)
	{
		delete m_server;
	}

	if (m_client)
	{
		delete m_client;
	}

	if (m_overhead)
	{
		delete m_terrain;
		delete m_overhead;
		delete m_visibility;
	}

	CLinkListNode<CServerInfo>* lln = m_serverInfo->m_list;

	while (lln->m_object)
	{
		delete lln->m_object->m_playerInfo->m_model;
		delete lln->m_object->m_playerInfo->m_camera;
		delete lln->m_object->m_playerInfo;

		lln = lln->m_next;
	}

	delete m_serverInfo;

	m_commandListBundle->Clear();

	delete m_commandListBundle;
}

/*
*/
void CEventManager::Process(CNetwork* network)
{
	CServerInfo* serverInfo = {};

	CPlayerInfo* playerInfo = (CPlayerInfo*)network->m_playerInfo;

	CLinkListNode<CServerInfo>* lln = m_serverInfo->Search(playerInfo->m_name);

	if (lln == nullptr)
	{
		CServerInfo* si = new CServerInfo((CServerInfo*)network->m_serverInfo);

		si->m_playerInfo = new CPlayerInfo();

		si->m_playerInfo->Initialize(playerInfo);

		m_serverInfo->Add(si, si->m_playerInfo->m_name);

		serverInfo = si;
	}
	else
	{
		lln->m_object->Initialize((CServerInfo*)network->m_serverInfo);

		lln->m_object->m_playerInfo->Initialize(playerInfo);

		serverInfo = lln->m_object;

		CString* name = new CString(lln->m_object->m_playerInfo->m_name);

		if (m_localPlayer != nullptr)
		{
			if (name->Compare(m_localPlayer->m_playerInfo->m_name) == 0)
			{
				m_localPlayer = lln->m_object;
			}
		}

		delete name;
	}

	pFunc[network->m_type](this, network, serverInfo);
}

/*
*/
void CEventManager::Send(CNetwork* network)
{
	if (network->m_audience == CNetwork::ClientEvent::E_CE_TO_LOCAL)
	{
		CEventManager::Process(network);

		return;
	}

	if (network->m_audience == CNetwork::ClientEvent::E_CE_TO_SERVER)
	{
		// Single player with bots
		if ((m_client == nullptr) && (m_server != nullptr))
		{
			m_server->ProcessEvent(network, m_server->m_frametime);


			// If this was a networked connection the serverInfo is handled when the player does a recv
			// in the client.  Because there is no client there is no recv so there is no update of the serverInfo.
			// All that happens here.  If the player does a local netowork command then the serverInfo is
			// updated during the CEventManager::Process(network); function above.
			CPlayerInfo* playerInfo = (CPlayerInfo*)network->m_playerInfo;

			CLinkListNode<CServerInfo>* llns = m_server->m_serverInfo->m_list;

			while (llns->m_object)
			{
				CLinkListNode<CServerInfo>* llnc = m_serverInfo->Search(llns->m_object->m_playerInfo->m_name);

				if (llnc == nullptr)
				{
					CServerInfo* si = new CServerInfo(llns->m_object);

					si->m_playerInfo = new CPlayerInfo();

					si->m_playerInfo->Initialize(llns->m_object->m_playerInfo);

					m_serverInfo->Add(si, si->m_playerInfo->m_name);

					// When the human player lload their environment the CEventManager_PlayerReady happens there and enter during that.
					// Bots are CEventManager_PlayerReady when they are added so enter them here when the local server is active.
					if (si->m_isBot)
					{
						CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_READY,
							nullptr, 0,
							(void*)si->m_playerInfo,
							(void*)si);

						CEventManager::Send(n);

						delete n;


						n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_PLAYER_ENTER,
							nullptr, 0,
							(void*)si->m_playerInfo,
							(void*)si);

						CEventManager::Send(n);

						delete n;


						CString* message = new CString(si->m_playerInfo->m_name);

						message->Concat(" entered");

						n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_CHAT,
							(char*)message->GetText(), message->GetLength(),
							(void*)si->m_playerInfo,
							(void*)si);

						CEventManager::Send(n);

						delete n;

						delete message;
					}
				}
				else
				{
					llnc->m_object->Initialize(llns->m_object);

					llnc->m_object->m_playerInfo->Initialize(llns->m_object->m_playerInfo);
				}

				llns = llns->m_next;
			}

			return;
		}

		// Multi-Player with bots
		if (m_client != nullptr)
		{
			if (m_client->m_isRunning)
			{
				m_client->Send(network);
			}

			return;
		}
	}
}

/*
*/
void CEventManager_Accepted(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_localPlayer = serverInfo;


	eventManager->m_client->m_connectionState = CClient::ClientState::E_CONNECTED;


	CString* message = new CString(serverInfo->m_playerInfo->m_name);

	message->Concat(" connection accepted");

	// Chat message
	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_CHAT,
		(void*)message->GetText(), message->GetLength(),
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	eventManager->m_client->Send(n);

	delete n;

	delete message;
}

/*
*/
void CEventManager_AccountInfo(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	CString* message = new CString((char*)network->m_data);

	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_CHAT,
		(void*)message->GetText(), message->GetLength(),
		(void*)serverInfo->m_playerInfo,
		nullptr);

	eventManager->Process(n);

	delete n;

	delete message;
}

/*
*/
void CEventManager_Chat(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_console->AddEventMessage("%s", (char*)network->m_data);
}

/*
*/
void CEventManager_ClientConnect(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	if (eventManager->m_client)
	{
		if (eventManager->m_client->m_connectionState == CClient::E_CONNECTED)
		{
			eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_ClientConnect:Already Connected\n");

			return;
		}

		delete eventManager->m_client;
	}

	eventManager->m_client = new CClient(eventManager->m_globalObjects, (char*)network->m_data);

	eventManager->m_client->m_hThread = (HANDLE)_beginthreadex(0, 0,
		&CEventManager_ReceiveThread,
		(void*)eventManager,
		0,
		&eventManager->m_client->m_receiveThreadId);
}

/*
*/
void CEventManager_Disconnect(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	if (eventManager->m_client)
	{
		if (eventManager->m_client->m_connectionState != CClient::E_CONNECTED)
		{
			eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_Disconnect:Not Connected\n");

			return;
		}
	}
	else
	{
		return;
	}

	serverInfo->m_playerInfo->m_isRecordable = false;


	CString* message = new CString(serverInfo->m_playerInfo->m_name);

	message->Concat(" disconnected");

	network->SetData(message->GetText(), message->GetLength());

	// Let this client know they are disconnecting
	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_CHAT,
		(void*)message->GetText(), message->GetLength(),
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	eventManager->Process(n);

	delete n;

	delete message;


	//  Sending a disconnect event
	n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_DISCONNECT,
		nullptr, 0,
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	eventManager->m_client->Send(n);

	delete n;


	eventManager->m_client->Disconnect();
}

/*
*/
void CEventManager_Exit(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	PostMessage(eventManager->m_globalObjects->m_window->m_hWnd, WM_DESTROY, 0, 0);
}

/*
*/
void CEventManager_LoadEnvironment(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_LoadEnvironment:%s\n", (char*)network->m_data);

	if (eventManager->m_overhead)
	{
		eventManager->m_commandListBundle->Clear();

		delete eventManager->m_terrain;
		delete eventManager->m_overhead;
	}

	eventManager->m_overhead = new COverhead(eventManager->m_globalObjects, CVertex2(256.0f, 256.0f), CVertex2(4.0f, 4.0f));

	eventManager->m_terrain = new CTerrain(eventManager->m_globalObjects, (char*)network->m_data);

	// Add and close the command lists so the textures can be uploaded
	eventManager->m_commandListBundle->Add(eventManager->m_terrain->m_commandList);
	eventManager->m_commandListBundle->Add(eventManager->m_overhead->m_commandList);





	// Environment cubes that contain some kind of visible item
	eventManager->m_visibility = new CVisibility(eventManager->m_terrain->m_width, eventManager->m_terrain->m_height, eventManager->m_server->m_serverEnvironment->m_gridUnits);

	CObject* collectable = new CObject(eventManager->m_globalObjects, eventManager->m_overhead,
		"model\\cube.wft", "model\\cube.mtl",
		"vertexPlayer.hlsl", "VSMain", "vs_5_0",
		"pixelPlayer.hlsl", "PSMain", "ps_5_0",
		false, true);

	collectable->SetScale(0.250f, 0.250f, 0.250f);
	collectable->SetPosition(5.0f, 0.50f, 5.0f);
	collectable->Update();

	collectable->m_floats[0]->m_values[0] = 2.0f;
	collectable->m_overheadFloats[0]->m_values[0] = 2.0f;

	// Determine which cube list to add the object.
	// Would need to test for all the vertices of the models making them contained in each block they belong with.
	
	// Grab the collectable list for the cube where the player is located
	int px = (int)(collectable->m_position.x + (eventManager->m_visibility->m_width / 2.0f)) / eventManager->m_visibility->m_gridUnits;
	int pz = (int)(collectable->m_position.z + (eventManager->m_visibility->m_height / 2.0f)) / eventManager->m_visibility->m_gridUnits;

	CLinkList<CObject>* cube = (CLinkList<CObject>*)eventManager->m_visibility->m_collectables->GetElement(2, px, pz);

	if (cube != nullptr)
	{
		// Need to perform the constructor when this object is the first one to be added.
		if (cube->m_list == nullptr)
		{
			cube->Constructor();
		}

		cube->Add(collectable, "item01");

		eventManager->m_commandListBundle->Add(collectable->m_commandList);
	}




	eventManager->m_commandListBundle->Close();

	// Must wait on the GPU for texture uploads to complete
	eventManager->m_globalObjects->m_videoDevice->WaitForGPU();

	eventManager->m_commandListBundle->Clear();


	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_READY,
		nullptr, 0,
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	eventManager->Send(n);

	delete n;


	// Request the other connected clients information
	n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_SEND_SERVER_INFO,
		nullptr, 0,
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	eventManager->Send(n);

	delete n;
}

/*
*/
void CEventManager_LocalConnect(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	serverInfo->m_isRunning = true;

	serverInfo->m_playerInfo->SetName("Steve");

	eventManager->m_localPlayer = serverInfo;

	eventManager->m_server->m_redTeamCount++;
}

/*
*/
void CEventManager_LocalLoadEnvironment(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_LocalLoadEnvironment:%s\n", (char*)network->m_data);

	if (eventManager->m_overhead)
	{
		eventManager->m_commandListBundle->Clear();

		delete eventManager->m_terrain;
		delete eventManager->m_overhead;
	}

	eventManager->m_server->m_serverEnvironment = new CServerEnvironment(eventManager->m_globalObjects, (char*)network->m_data);

	eventManager->m_server->m_collision->m_environment = eventManager->m_server->m_serverEnvironment;


	eventManager->m_overhead = new COverhead(eventManager->m_globalObjects, CVertex2(256.0f, 256.0f), CVertex2(4.0f, 4.0f));

	eventManager->m_terrain = new CTerrain(eventManager->m_globalObjects, (char*)network->m_data);

	// Add and close the command lists so the textures can be uploaded
	eventManager->m_commandListBundle->Add(eventManager->m_terrain->m_commandList);
	eventManager->m_commandListBundle->Add(eventManager->m_overhead->m_commandList);

	
	
	
	// Environment cubes that contain some kind of visible item
	eventManager->m_visibility = new CVisibility(eventManager->m_terrain->m_width, eventManager->m_terrain->m_height, eventManager->m_server->m_serverEnvironment->m_gridUnits);

	CObject* collectable = new CObject(eventManager->m_globalObjects, eventManager->m_overhead,
		"model\\cube.wft", "model\\cube.mtl",
		"vertexPlayer.hlsl", "VSMain", "vs_5_0",
		"pixelPlayer.hlsl", "PSMain", "ps_5_0",
		false, true);

	collectable->SetScale(0.250f, 0.250f, 0.250f);
	collectable->SetPosition(5.0f, 0.50f, 5.0f);
	collectable->Update();

	collectable->m_floats[0]->m_values[0] = 2.0f;
	collectable->m_overheadFloats[0]->m_values[0] = 2.0f;

	// Determine which cube list to add the object.
	// Would need to test for all the vertices of the models making them contained in each block they belong with.
	CLinkList<CObject>* cube = (CLinkList<CObject>*)eventManager->m_visibility->m_collectables->GetElement(2, collectable->m_position.x, collectable->m_position.z);

	if (cube != nullptr)
	{
		// Need to perform the constructor when this object is the first one to be added.
		if (cube->m_list == nullptr)
		{
			cube->Constructor();
		}

		cube->Add(collectable, "item01");

		eventManager->m_commandListBundle->Add(collectable->m_commandList);
	}



	
	
	eventManager->m_commandListBundle->Close();

	// Must wait on the GPU for texture uploads to complete
	eventManager->m_globalObjects->m_videoDevice->WaitForGPU();

	eventManager->m_commandListBundle->Clear();



	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_READY,
		nullptr, 0,
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	eventManager->Send(n);

	delete n;


	n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_PLAYER_ENTER,
		nullptr, 0,
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	eventManager->Send(n);

	delete n;


	CString* message = new CString(serverInfo->m_playerInfo->m_name);

	message->Concat(" entered");

	n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_CHAT,
		(char*)message->GetText(), message->GetLength(),
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	eventManager->Send(n);

	delete n;

	delete message;
}

/*
*/
void CEventManager_LocalStartServer(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_server = new CServer(eventManager->m_globalObjects);

	eventManager->m_server->StartServer();
}

/*
*/
void CEventManager_PlayerAttack(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_globalObjects->m_soundMgr->Get("audio\\chimes.wav")->StartSound();
}

/*
*/
void CEventManager_PlayerEnter(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_PlayerEnter:%s\n", serverInfo->m_playerInfo->m_name);


	serverInfo->m_playerInfo->m_camera = new CCamera((float)eventManager->m_globalObjects->m_window->m_width,
		(float)eventManager->m_globalObjects->m_window->m_height,
		&serverInfo->m_playerInfo->m_position,
		45.0f,
		1.0f, 50000.0f);

	serverInfo->m_playerInfo->m_camera->UpdateRotation(
		serverInfo->m_playerInfo->m_direction.p.x,
		serverInfo->m_playerInfo->m_direction.p.y,
		serverInfo->m_playerInfo->m_direction.p.z);

	serverInfo->m_playerInfo->m_camera->UpdateView();

	serverInfo->m_playerInfo->SetDirection(&serverInfo->m_playerInfo->m_camera->m_look);

	serverInfo->m_playerInfo->m_model = new CObject(eventManager->m_globalObjects, eventManager->m_overhead,
		"model\\cube.wft", "model\\cube.mtl",
		"vertexPlayer.hlsl", "VSMain", "vs_5_0",
		"pixelPlayer.hlsl", "PSMain", "ps_5_0",
		false, true);

	switch (serverInfo->m_playerInfo->m_team)
	{
	case CPlayerInfo::E_TEAM_RED:
	{
		serverInfo->m_playerInfo->m_model->m_floats[0]->m_values[0] = 0.0f;
		serverInfo->m_playerInfo->m_model->m_overheadFloats[0]->m_values[0] = 0.0f;

		break;
	}
	case CPlayerInfo::E_TEAM_BLUE:
	{
		serverInfo->m_playerInfo->m_model->m_floats[0]->m_values[0] = 1.0f;
		serverInfo->m_playerInfo->m_model->m_overheadFloats[0]->m_values[0] = 1.0f;

		break;
	}
	}

	// Must wait on the GPU for any texture uploads to complete
	eventManager->m_globalObjects->m_videoDevice->WaitForGPU();


	serverInfo->m_playerInfo->m_isRecordable = true;


	if (eventManager->m_server->m_localServerRunning)
	{
		return;
	}


	CString* message = new CString(serverInfo->m_playerInfo->m_name);

	message->Concat(" entered");

	// Chat message to update this player's info on the server
	CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_CHAT,
		(void*)message->GetText(), message->GetLength(),
		(void*)serverInfo->m_playerInfo,
		(void*)serverInfo);

	eventManager->m_client->Send(n);

	delete n;

	delete message;
}

/*
*/
void CEventManager_PlayerExit(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_PlayerExit:%s\n",
		serverInfo->m_playerInfo->m_name);
}

/*
*/
void CEventManager_PlayerInfoRequest(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	serverInfo->m_playerInfo->m_camera = new CCamera((float)eventManager->m_globalObjects->m_window->m_width,
		(float)eventManager->m_globalObjects->m_window->m_height,
		&serverInfo->m_playerInfo->m_position,
		45.0f,
		1.0f, 50000.0f);

	serverInfo->m_playerInfo->m_model = new CObject(eventManager->m_globalObjects, eventManager->m_overhead,
		"model\\cube.wft", "model\\cube.mtl",
		"vertexPlayer.hlsl", "VSMain", "vs_5_0",
		"pixelPlayer.hlsl", "PSMain", "ps_5_0",
		false, true);

	// Must wait on the GPU for any texture uploads to complete
	eventManager->m_globalObjects->m_videoDevice->WaitForGPU();


	serverInfo->m_playerInfo->m_isRecordable = true;
}

/*
*/
void CEventManager_PlayerUpdate(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	if (!serverInfo->m_playerInfo->m_isRecordable)
	{
		return;
	}

	serverInfo->m_playerInfo->m_model->SetPosition(&serverInfo->m_playerInfo->m_position);

	serverInfo->m_playerInfo->m_model->m_rotation.y = serverInfo->m_playerInfo->m_direction.PointToDegree().p.y;

	serverInfo->m_playerInfo->m_model->Update();

	serverInfo->m_playerInfo->m_camera->SetPosition(&serverInfo->m_playerInfo->m_position);
}

/*
*/
void CEventManager_ServerInfo(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	return;
}

/*
*/
void CEventManager_StartServer(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_StartServer\n");

	if (eventManager->m_server)
	{
		if (eventManager->m_server->m_listenThreadRunning)
		{
			eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_StartServer:Server Already Running\n");

			return;
		}

		delete eventManager->m_server;
	}

	eventManager->m_server = new CServer(eventManager->m_globalObjects);

	eventManager->m_server->StartServer((char*)network->m_data);
}

/*
*/
void CEventManager_StopServer(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_StopServer\n");

	if (!eventManager->m_server)
	{
		eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_StopServer:Server Not Running\n");

		return;
	}


	if (eventManager->m_server->m_localServerRunning)
	{
		eventManager->m_server->ShutdownLocal();

		return;
	}

	eventManager->m_server->Shutdown();
}

/*
*/
void CEventManager_ToggleConsole(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_console->ToggleVisibility();
}

/*
*/
void CEventManager_ToggleWindowmode(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	eventManager->m_globalObjects->m_videoDevice->ToggleFullScreenWindow();
}

/*
*/
void CEventManager_UpdateCollectable(CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo)
{
	int px = (int)(serverInfo->m_playerInfo->m_position.p.x + (eventManager->m_visibility->m_width / 2.0f)) / eventManager->m_visibility->m_gridUnits;
	int pz = (int)(serverInfo->m_playerInfo->m_position.p.z + (eventManager->m_visibility->m_height / 2.0f)) / eventManager->m_visibility->m_gridUnits;

	CLinkList<CObject>* collectables = (CLinkList<CObject>*)eventManager->m_visibility->m_collectables->GetElement(2, px, pz);

	if (collectables != nullptr)
	{
		if (collectables->m_count > 0)
		{
			CLinkListNode<CObject>* collectable = collectables->Search((char*)network->m_data);

			if (collectable != nullptr)
			{
				collectable->m_object->m_limboTimer->Start();
			}
		}
	}
}

/*
* When connected to a remote server
*/
unsigned __stdcall CEventManager_ReceiveThread(void* obj)
{
	CEventManager* em = (CEventManager*)obj;

#ifdef _DEBUG
	em->m_globalObjects->m_errorLog->WriteError("CEventManager_ReceiveThread:Starting\n");
#endif

	CClient* client = em->m_client;

	client->m_isRunning = true;

	while (client->m_isRunning)
	{
		if (client->m_connectionState == CClient::E_NOTCONNECTED)
		{
			client->m_isRunning = false;
		}
		else
		{
			client->Receive();

			if (client->m_connectionState == CClient::E_NOTCONNECTED)
			{
				client->m_isRunning = false;
			}
			else
			{
				em->Process(client->m_network);
			}
		}
	}

#ifdef _DEBUG
	em->m_globalObjects->m_errorLog->WriteError("CEventManager_ReceiveThread Ending\n");
#endif

	_endthreadex(0);

	return 0;
}
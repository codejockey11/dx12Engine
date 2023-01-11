#include "CEventManager.h"

// Network thread for client receive events
unsigned __stdcall CEventManager_ClientReceiveThread(void* obj);

// Event manager functions
void CEventManager_Accepted(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_Attack(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_Chat(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_ClientConnect(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_Disconnect(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_DrawFrame(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_Exit(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_LoadEnvironment(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_PlayerEnter(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_PlayerExit(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_PlayerUpdate(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_StartServer(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_StopServer(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_ToggleConsole(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);
void CEventManager_Windowmode(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);


CEventManager::CEventManager()
{
	memset(this, 0x00, sizeof(CEventManager));
}

CEventManager::CEventManager(CGlobalObjects* globalObjects, CConsole* console)
{
	memset(this, 0x00, sizeof(CEventManager));

	m_globalObjects = globalObjects;

	m_console = console;

	m_commandListBundle = new CCommandListBundle(m_globalObjects->m_errorLog, m_globalObjects->m_videoDevice);


	pFunc[CEvent::ET_ACCEPTED] = &CEventManager_Accepted;
	pFunc[CEvent::ET_ATTACK] = &CEventManager_Attack;
	pFunc[CEvent::ET_CHAT] = &CEventManager_Chat;
	pFunc[CEvent::ET_CONNECT] = &CEventManager_ClientConnect;
	pFunc[CEvent::ET_DISCONNECT] = &CEventManager_Disconnect;
	pFunc[CEvent::ET_DRAW] = &CEventManager_DrawFrame;
	pFunc[CEvent::ET_EXIT] = &CEventManager_Exit;
	pFunc[CEvent::ET_LOADENVIRONMENT] = &CEventManager_LoadEnvironment;
	pFunc[CEvent::ET_PLAYERENTER] = &CEventManager_PlayerEnter;
	pFunc[CEvent::ET_PLAYEREXIT] = &CEventManager_PlayerExit;
	pFunc[CEvent::ET_PLAYERUPDATE] = &CEventManager_PlayerUpdate;
	pFunc[CEvent::ET_STARTSERVER] = &CEventManager_StartServer;
	pFunc[CEvent::ET_STOPSERVER] = &CEventManager_StopServer;
	pFunc[CEvent::ET_TOGGLECONSOLE] = &CEventManager_ToggleConsole;
	pFunc[CEvent::ET_WINDOWMODE] = &CEventManager_Windowmode;
}

CEventManager::~CEventManager()
{
	if (m_overhead)
	{
		delete m_light;
		delete m_dynamicModel;
		delete m_staticModel;
		delete m_wavefront;
		delete m_terrain;
		delete m_overhead;
	}

	delete m_redTeamInfo[0];

	if (m_client)
	{
		delete m_client;
	}

	if (m_server)
	{
		delete m_server;
	}

	m_commandListBundle->Clear();
	
	delete m_commandListBundle;
}

void CEventManager::Send(CEvent* evt)
{
	if (evt->m_audience == CEvent::ET_LOCAL)
	{
		CEventManager::Process(evt);

		return;
	}

	if ((m_client == nullptr) && (evt->m_audience == CEvent::ET_SERVER))
	{
		CEventManager::Process(evt);

		return;
	}

	if ((m_client != nullptr) && (evt->m_audience == CEvent::ET_SERVER))
	{
		if (m_client->m_isRunning)
		{
			m_client->SendEvent(evt);
		}
		else
		{
			delete m_client;

			m_client = nullptr;
		}

		return;
	}
}

void CEventManager::Process(CEvent* evt)
{
	if (evt->m_type == CEvent::ET_EMPTY)
	{
		return;
	}

	int i = 0;

	pFunc[evt->m_type](this, evt, (CPlayerInfo*)evt->m_playerInfo);
}

void CEventManager_Accepted(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_Accepted:%i\n", evt->m_fromClient);

	eventManager->m_client->m_clientNbr = evt->m_toClient;

	eventManager->m_client->m_connectionState = CClient::CS_CONNECTED;

	
	eventManager->m_redTeamInfo[playerInfo->m_number] = new CPlayerInfo();

	memcpy((void*)eventManager->m_redTeamInfo[playerInfo->m_number], (void*)playerInfo, sizeof(CPlayerInfo));

	memcpy((void*)eventManager->m_redTeamInfo[playerInfo->m_number]->m_name, (void*)"Steve", 5);


	CEvent* clientEvent = new CEvent(eventManager->m_redTeamInfo[playerInfo->m_number]->m_number, -1, CEvent::ET_SERVER, CEvent::CServerEvent::SE_UPDATEPLAYERINFO, nullptr, 0, (void*)eventManager->m_redTeamInfo[playerInfo->m_number]);

	eventManager->m_client->SendEvent(clientEvent);

	delete clientEvent;
}

void CEventManager_Attack(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	memcpy((void*)eventManager->m_redTeamInfo[playerInfo->m_number], (void*)playerInfo, sizeof(CPlayerInfo));

	eventManager->m_globalObjects->m_soundMgr->GetSound("audio\\chimes.wav")->StartSound();
}

void CEventManager_Chat(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	memcpy((void*)eventManager->m_redTeamInfo[playerInfo->m_number], (void*)playerInfo, sizeof(CPlayerInfo));

	eventManager->m_globalObjects->m_errorLog->WriteError("%i %s\n", evt->m_fromClient, evt->m_data);

	eventManager->m_console->AddEventMessage("%i %s", evt->m_fromClient, evt->m_data);
}

void CEventManager_ClientConnect(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	if (eventManager->m_client)
	{
		if (eventManager->m_client->m_connectionState == CClient::CS_CONNECTED)
		{
			eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_ClientConnect:Already Connected:%i\n", evt->m_fromClient);

			return;
		}

		delete eventManager->m_client;
	}

	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_ClientConnect\n");

	eventManager->m_client = new CClient(eventManager->m_globalObjects, evt);

	eventManager->m_client->m_hThread = (HANDLE)_beginthreadex(NULL, 0, &CEventManager_ClientReceiveThread, (void*)eventManager, 0, &eventManager->m_client->m_receiveThreadId);
}

void CEventManager_Disconnect(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	if (eventManager->m_client)
	{
		if (eventManager->m_client->m_connectionState != CClient::CS_CONNECTED)
		{
			eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_Disconnect:Not Connected\n");

			return;
		}
	}
	else
	{
		return;
	}

	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_Disconnect:%i\n", evt->m_fromClient);

	//  Sending a disconnect event
	CEvent* clientEvent = new CEvent(playerInfo->m_number, -1, CEvent::ET_SERVER, CEvent::CServerEvent::SE_DISCONNECT, nullptr, 0, (void*)playerInfo);

	eventManager->m_client->SendEvent(clientEvent);

	delete clientEvent;


	eventManager->m_client->Disconnect();
}

void CEventManager_DrawFrame(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	PostMessage(eventManager->m_globalObjects->m_window->m_hWnd, CEvent::WM_DRAW, 0, 0);
}

void CEventManager_Exit(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_Exit\n");

	if (eventManager->m_client)
	{
		if (eventManager->m_client->m_connectionState == CClient::CS_CONNECTED)
		{
			eventManager->m_client->Disconnect();
		}
	}

	if (eventManager->m_server)
	{
		if (eventManager->m_server->m_acceptRunning)
		{
			eventManager->m_server->Shutdown();
		}
	}

	PostMessage(eventManager->m_globalObjects->m_window->m_hWnd, WM_DESTROY, 0, 0);
}

void CEventManager_LoadEnvironment(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_LoadEnvironment:%s\n", (char*)evt->m_data);

	memcpy((void*)eventManager->m_redTeamInfo[playerInfo->m_number], (void*)playerInfo, sizeof(CPlayerInfo));


	if (eventManager->m_overhead)
	{
		eventManager->m_commandListBundle->Clear();

		delete eventManager->m_light;
		delete eventManager->m_dynamicModel;
		delete eventManager->m_staticModel;
		delete eventManager->m_terrain;
		delete eventManager->m_overhead;
	}

	eventManager->m_overhead = new COverhead(eventManager->m_globalObjects, CVertex2(256.0f, 256.0f), CVertex2(4.0f, 4.0f));

	eventManager->m_terrain = new CTerrain(eventManager->m_globalObjects, (char*)evt->m_data);

	eventManager->m_wavefront = new CObject(eventManager->m_globalObjects, eventManager->m_overhead, "model\\Residential Buildings 010.obj", "model\\ResidentialBuildings.mtl", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", false, true);

	eventManager->m_wavefront->isDynamic = true;

	eventManager->m_wavefront->m_scale.x = 0.1250f;
	eventManager->m_wavefront->m_scale.y = 0.1250f;
	eventManager->m_wavefront->m_scale.z = 0.1250f;

	eventManager->m_wavefront->m_position.x = 8.0f;
	eventManager->m_wavefront->m_position.y = 0.0f;
	eventManager->m_wavefront->m_position.z = 8.0f;

	eventManager->m_wavefront->m_rotation.y = -45.0f;
	eventManager->m_wavefront->Update();

	eventManager->m_wavefront->isDynamic = false;


	eventManager->m_dynamicModel = new CObject(eventManager->m_globalObjects, eventManager->m_overhead, "model\\cube.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", false, true);
	eventManager->m_staticModel = new CObject(eventManager->m_globalObjects, eventManager->m_overhead, "model\\cube.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", false, true);
	//eventManager->m_model = new CObject(eventManager->m_globalObjects, eventManager->m_overhead, "woodenwatchtower2.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", false, true);
	//eventManager->m_model = new CObject(eventManager->m_globalObjects, eventManager->m_overhead, "OldHouse.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", true, false);
	//eventManager->m_model = new CObject(eventManager->m_globalObjects, eventManager->m_overhead, "ResidentialBuildings001.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", false, true);
	//eventManager->m_model = new CObject(eventManager->m_globalObjects, eventManager->m_overhead, "ResidentialBuildings005.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", false, true);
	//eventManager->m_model = new CObject(eventManager->m_globalObjects, eventManager->m_overhead, "ResidentialBuildings010.model", "vertexModel.hlsl", "VSMain", "vs_5_0", "pixelModel.hlsl", "PSMain", "ps_5_0", false, true);

	eventManager->m_dynamicModel->isDynamic = true;

	eventManager->m_dynamicModel->m_scale.x = 1.0f;
	eventManager->m_dynamicModel->m_scale.y = 1.0f;
	eventManager->m_dynamicModel->m_scale.z = 1.0f;

	eventManager->m_dynamicModel->m_position.x = 0.0f;
	eventManager->m_dynamicModel->m_position.y = 1.0f;

	eventManager->m_dynamicModel->m_rotation.y = 45.0f;
	eventManager->m_dynamicModel->Update();

	
	// One time dynamic to update the initial vertex buffers
	eventManager->m_staticModel->isDynamic = true;

	eventManager->m_staticModel->m_scale.x = 0.250f;
	eventManager->m_staticModel->m_scale.y = 0.250f;
	eventManager->m_staticModel->m_scale.z = 0.250f;

	eventManager->m_staticModel->m_position.x = 8.0f;
	eventManager->m_staticModel->m_position.y = 1.0f;


	eventManager->m_staticModel->Update();

	eventManager->m_staticModel->isDynamic = false;

	CVertex diffuse(ZombieGreen.m_RGBA[0], ZombieGreen.m_RGBA[1], ZombieGreen.m_RGBA[2]);
	//CVertex diffuse(1.0f, 1.0f, 1.0f);
	CVertex position(32.0f, 16.0f, 0.0f);
	CVertex direction(0.0f, -1.0f, 0.0f);

	eventManager->m_light = new CLight((float)eventManager->m_globalObjects->m_window->m_width, (float)eventManager->m_globalObjects->m_window->m_height,
		45.0f, 1.0f, 50000.0f,
		&diffuse,
		&position,
		&direction,
		32.0f);

	// Add and close the command lists so the textures can be uploaded
	eventManager->m_commandListBundle->Add(eventManager->m_terrain->m_commandList);
	eventManager->m_commandListBundle->Add(eventManager->m_dynamicModel->m_commandList);
	eventManager->m_commandListBundle->Add(eventManager->m_staticModel->m_commandList);
	eventManager->m_commandListBundle->Add(eventManager->m_wavefront->m_commandList);
	eventManager->m_commandListBundle->Add(eventManager->m_overhead->m_commandList);

	eventManager->m_commandListBundle->Close();

	// Must wait on the GPU for texture uploads to complete
	eventManager->m_globalObjects->m_videoDevice->WaitForGPU();

	eventManager->m_commandListBundle->Clear();

	
	CEvent* clientEvent = new CEvent(-1, eventManager->m_redTeamInfo[playerInfo->m_number]->m_number, CEvent::ET_SERVER, CEvent::CServerEvent::SE_PLAYERREADY, nullptr, 0, (void*)eventManager->m_redTeamInfo[playerInfo->m_number]);

	eventManager->Send(clientEvent);

	delete clientEvent;
}

void CEventManager_PlayerEnter(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	memcpy((void*)eventManager->m_redTeamInfo[playerInfo->m_number], (void*)playerInfo, sizeof(CPlayerInfo));

	if (eventManager->m_redTeamCount == CEventManager::MAX_PLAYERS)
	{
		eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_PlayerEnter:Team at max players\n");

		return;
	}

	CPlayer* player = new CPlayer(eventManager->m_globalObjects, &eventManager->m_redTeamInfo[playerInfo->m_number]->m_position, &eventManager->m_redTeamInfo[playerInfo->m_number]->m_direction, eventManager->m_terrain, CPlayer::PT_RED, 8.0f, (char*)playerInfo->m_name);

	for (int i = 0; i < CEventManager::MAX_PLAYERS; i++)
	{
		if (eventManager->m_redTeam[i] == nullptr)
		{
			eventManager->m_redTeam[i] = player;

			eventManager->m_redTeamCount++;

			break;
		}
	}

	eventManager->m_redTeamInfo[playerInfo->m_number]->m_isReady = true;

	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_PlayerEnter:%s\n", eventManager->m_redTeamInfo[playerInfo->m_number]->m_name);
}

void CEventManager_PlayerExit(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	memcpy((void*)eventManager->m_redTeamInfo[playerInfo->m_number], (void*)playerInfo, sizeof(CPlayerInfo));

	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_PlayerExit:%s\n", eventManager->m_redTeamInfo[playerInfo->m_number]->m_name);
}

void CEventManager_PlayerUpdate(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	if (eventManager->m_redTeam[playerInfo->m_number] != nullptr)
	{
		memcpy((void*)eventManager->m_redTeamInfo[playerInfo->m_number], (void*)playerInfo, sizeof(CPlayerInfo));

		eventManager->m_redTeam[playerInfo->m_number]->m_camera->m_position.x = eventManager->m_redTeamInfo[playerInfo->m_number]->m_position.p.x;
		eventManager->m_redTeam[playerInfo->m_number]->m_camera->m_position.y = eventManager->m_redTeamInfo[playerInfo->m_number]->m_position.p.y;
		eventManager->m_redTeam[playerInfo->m_number]->m_camera->m_position.z = eventManager->m_redTeamInfo[playerInfo->m_number]->m_position.p.z;
	}
}

void CEventManager_StartServer(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	if (eventManager->m_server)
	{
		if (eventManager->m_server->m_acceptRunning)
		{
			eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_StartServer:Server Already Running\n");

			return;
		}

		delete eventManager->m_server;
	}

	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_StartServer\n");

	eventManager->m_server = new CServer(eventManager->m_globalObjects, (char*)evt->m_data);
}

void CEventManager_StopServer(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	if (!eventManager->m_server)
	{
		eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_StopServer:Server Not Running\n");

		return;
	}

	eventManager->m_globalObjects->m_errorLog->WriteError("CEventManager_StopServer\n");

	eventManager->m_server->Shutdown();
}

void CEventManager_ToggleConsole(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	eventManager->m_console->ToggleVisibility();
}

void CEventManager_Windowmode(CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo)
{
	eventManager->m_globalObjects->m_videoDevice->ToggleFullscreenWindow();
}

unsigned __stdcall CEventManager_ClientReceiveThread(void* obj)
{
	CEventManager* em = (CEventManager*)obj;

	CClient* client = em->m_client;

	client->m_isRunning = true;

#ifdef _DEBUG
	em->m_globalObjects->m_errorLog->WriteError("CEventManager_ClientReceiveThread:Starting\n");
#endif

	while (client->m_isRunning)
	{
		if (client->m_connectionState == CClient::CS_NOTCONNECTED)
		{
			client->m_isRunning = false;
		}
		else
		{
			client->ClientReceive();

			if (client->m_connectionState == CClient::CS_NOTCONNECTED)
			{
				client->m_isRunning = false;
			}
			else
			{
				em->Process(client->m_event);
			}
		}
	}

#ifdef _DEBUG
	em->m_globalObjects->m_errorLog->WriteError("CEventManager_ClientReceiveThread:Ending\n");
#endif

	_endthreadex(0);

	return 0;
}
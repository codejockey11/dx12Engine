#pragma once

#include "standard.h"

#include "CClient.h"
#include "CColor.h"
#include "CCommandListBundle.h"
#include "CConsole.h"
#include "CGlobalObjects.h"
#include "CEvent.h"
#include "CLight.h"
#include "CLinkList.h"
#include "CObject.h"
#include "CWavefront.h"
#include "COverhead.h"
#include "CPlayer.h"
#include "CPlayerInfo.h"
#include "CServer.h"
#include "CString.h"
#include "CTeamStart.h"
#include "CTerrain.h"

class CEventManager
{
public:

	enum
	{
		MAX_PLAYERS = 5
	};

	CGlobalObjects* m_globalObjects;

	CServer* m_server;

	CClient* m_client;

	CEvent* m_events[CEvent::ET_MAX];

	CConsole* m_console;

	CPlayer* m_redTeam[CEventManager::MAX_PLAYERS];
	CPlayerInfo* m_redTeamInfo[CEventManager::MAX_PLAYERS];

	int m_redTeamCount;

	CPlayer* m_blueTeam[CEventManager::MAX_PLAYERS];
	CPlayerInfo* m_blueTeamInfo[CEventManager::MAX_PLAYERS];

	int m_blueTeamCount;


	CTerrain* m_terrain;

	CObject* m_dynamicModel;
	CObject* m_staticModel;

	COverhead* m_overhead;

	CObject* m_wavefront;

	CLight* m_light;

	CCommandListBundle* m_commandListBundle;


	void (*pFunc[CEvent::ET_MAX]) (CEventManager* eventManager, CEvent* evt, CPlayerInfo* playerInfo);


	CEventManager();

	CEventManager(CGlobalObjects* globalObjects, CConsole* console);

	~CEventManager();

	
	void Send(CEvent* evt);

	void Process(CEvent* evt);
};
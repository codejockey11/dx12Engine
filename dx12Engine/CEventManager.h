#pragma once

#include "standard.h"

#include "CClient.h"
#include "CColor.h"
#include "CCommandListBundle.h"
#include "CConsole.h"
#include "CGlobalObjects.h"
#include "CLight.h"
#include "CLinkList.h"
#include "CNetwork.h"
#include "CObject.h"
#include "COverhead.h"
#include "CServer.h"
#include "CServerInfo.h"
#include "CString.h"
#include "CTeamStart.h"
#include "CTerrain.h"
#include "CVisibility.h"
#include "CWavefront.h"

class CEventManager
{
public:

	CClient* m_client;
	CCommandListBundle* m_commandListBundle;
	CConsole* m_console;
	CGlobalObjects* m_globalObjects;
	CLinkList<CServerInfo>* m_serverInfo;
	COverhead* m_overhead;
	CServer* m_server;
	CServerInfo* m_localPlayer;
	CTerrain* m_terrain;
	CVisibility* m_visibility;

	void (*pFunc[CNetwork::ClientEvent::E_CE_MAX_EVENTS]) (CEventManager* eventManager, CNetwork* network, CServerInfo* serverInfo);

	CEventManager();
	CEventManager(CGlobalObjects* globalObjects, CConsole* console);
	~CEventManager();

	void Process(CNetwork* network);
	void Send(CNetwork* network);
};
#pragma once

#include "standard.h"

#include "CAccountInfo.h"
#include "CCollision.h"
#include "CFrametime.h"
#include "CGlobalObjects.h"
#include "CLinkList.h"
#include "CNetwork.h"
#include "CPlayerInfo.h"
#include "CServerInfo.h"
#include "CTimer.h"

class CServer
{
public:

	CCollision* m_collision;
	CFrametime* m_frametime;
	CGlobalObjects* m_globalObjects;
	CLinkList<CServerInfo>* m_serverInfo;
	CServerEnvironment* m_serverEnvironment;
	
	SOCKET m_listenSocket;
	
	bool m_listenThreadRunning;
	bool m_localServerRunning;
	
	int m_blueTeamCount;
	int m_redTeamCount;

	void (*pFunc[CNetwork::ServerEvent::E_SE_MAX_EVENTS]) (CServer* server, CNetwork* network, CServerInfo* serverInfo);

	CServer();
	CServer(CGlobalObjects* globalObjects);
	~CServer();

	void Accept();
	void BotInference(CServerInfo* serverInfo, CFrametime* frametime);
	void CreateListenSocket(const char* port);
	void CreatePlayer(CNetwork* network, bool isHuman, SOCKET tempSocket);
	void InitializeWinsock();
	void ProcessEvent(CNetwork* network, CFrametime* frametime);
	void ReceivePlayer(CServerInfo* serverInfo, CFrametime* frametime);
	void SendNetwork(CNetwork* network);
	void SendNetwork(CNetwork* network, SOCKET tempSocket);
	void Shutdown();
	void ShutdownListen();
	void ShutdownLocal();
	void ShutdownPlayers();
	void StartListenSocket();
	void StartServer();
	void StartServer(const char* port);

private:

	CNetwork* m_network;
	
	HANDLE m_listenThreadHandle;
	
	UINT m_listenThreadId;
	
	WSADATA	wsaData;
};
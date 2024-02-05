#pragma once

#include "standard.h"

#include "CCamera.h"
#include "CPlayerInfo.h"
#include "CTimer.h"

class CServerInfo
{
public:

	CCamera* m_botCamera;
	CPlayerInfo* m_playerInfo;
	CTimer* m_timerReload;
	
	HANDLE m_hThread;
	
	SOCKET m_socket;
	
	UINT m_threadId;
	
	bool m_isBot;
	bool m_isFalling;
	bool m_isRunning;
	
	float m_freefallVelocity;
	float m_idleTime;
	float m_velocity;
	
	void* m_server;

	CServerInfo();
	CServerInfo(CServerInfo* serverInfo);
	~CServerInfo();

	void Initialize(CServerInfo* serverInfo);
};
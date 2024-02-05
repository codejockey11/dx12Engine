#pragma once

#include "standard.h"

class CNetwork
{
public:

	enum ClientEvent
	{
		E_CE_ACCEPTED = 0,
		E_CE_ACCOUNT_INFO,
		E_CE_CHAT,
		E_CE_CONNECT,
		E_CE_DISCONNECT,
		E_CE_DRAW_FRAME,
		E_CE_EXIT_GAME,
		E_CE_LOAD_ENVIRONMENT,
		E_CE_LOCAL_CONNECT,
		E_CE_LOCAL_LOAD_ENVIRONMENT,
		E_CE_LOCAL_START_SERVER,
		E_CE_OPEN_BROWSER,
		E_CE_PLAYER_ATTACK,
		E_CE_PLAYER_BACKWARD,
		E_CE_PLAYER_ENTER,
		E_CE_PLAYER_EXIT,
		E_CE_PLAYER_FIRST_PERSON_MOVE,
		E_CE_PLAYER_FORWARD,
		E_CE_PLAYER_INFO,
		E_CE_PLAYER_STOP,
		E_CE_PLAYER_STRAFE_LEFT,
		E_CE_PLAYER_STRAFE_RIGHT,
		E_CE_PLAYER_UPDATE,
		E_CE_SERVER_INFO,
		E_CE_START_SERVER,
		E_CE_STOP_SERVER,
		E_CE_TOGGLE_CONSOLE,
		E_CE_TOGGLE_WINDOW,
		E_CE_TO_CLIENT,
		E_CE_TO_LOCAL,
		E_CE_TO_SERVER,
		E_CE_UPDATE_COLLECTABLE,

		E_CE_MAX_EVENTS
	};

	enum ServerEvent
	{
		E_SE_ADD_BOT = 0,
		E_SE_BOT_MOVE,
		E_SE_CHAT,
		E_SE_DISCONNECT,
		E_SE_LOAD_ENVIRONMENT,
		E_SE_PLAYER_IDLE,
		E_SE_PLAYER_MOVE,
		E_SE_PLAYER_READY,
		E_SE_SEND_SERVER_INFO,

		E_SE_MAX_EVENTS
	};

	enum
	{
		E_DATA_SIZE = 1024,
		E_PLAYER_INFO_SIZE = 128,	// 104 = sizeof(CPlayerinfo)
		E_SERVER_INFO_SIZE = 128	// 80 = sizeof(CServerinfo)
	};

	BYTE m_audience;
	BYTE m_data[CNetwork::E_DATA_SIZE];
	BYTE m_playerInfo[CNetwork::E_PLAYER_INFO_SIZE];
	BYTE m_serverInfo[CNetwork::E_SERVER_INFO_SIZE];
	BYTE m_type;

	int m_length;

	CNetwork();
	CNetwork(BYTE audience, BYTE type, void* data, int length, void* playerInfo, void* serverInfo);
	~CNetwork();

	void SetData(void* data, int length);
	void SetPlayerInfo(void* playerInfo);
	void SetServerInfo(void* serverInfo);
};
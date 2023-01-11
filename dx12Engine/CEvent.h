#pragma once

#include "standard.h"

class CEvent
{
public:

	enum
	{
		ET_EMPTY = 0,

		ET_PLAYERENTER,
		ET_PLAYEREXIT,
		ET_PLAYERUPDATE,

		ET_CONNECT,
		ET_DISCONNECT,

		ET_STARTSERVER,
		ET_STOPSERVER,
		ET_ACCEPTED,

		ET_TOGGLECONSOLE,

		ET_CHAT,

		ET_CLIENT,
		ET_TEAM,
		ET_SERVER,
		ET_LOCAL,

		ET_FORWARD,
		ET_BACKWARD,
		ET_STRAFELEFT,
		ET_STRAFERIGHT,

		ET_ATTACK,

		ET_RENDERFRAME,
		ET_WINDOWMODE,

		ET_LOADENVIRONMENT,

		ET_FPSMOVE,

		ET_EXIT,

		ET_DRAW,
		ET_FRAMESTART,

		ET_MAX,

		WM_DRAW = 5000,

		// playerinfo length = 1121
		DATA_LENGTH = 1200
	};

	enum CServerEvent
	{
		SE_UPDATEPLAYERINFO = 0,
		SE_DISCONNECT,
		SE_IDLEPLAYER,
		SE_LOADENVIRONMENT,
		SE_PLAYERREADY,
		SE_PLAYERMOVE,

		SE_MAX,

		SE_TEAM_RED = 0
	};

	int m_fromClient;
	int	m_toClient;

	BYTE m_audience;
	BYTE m_type;

	int	m_length;

	char m_data[CEvent::DATA_LENGTH];

	char m_playerInfo[CEvent::DATA_LENGTH];


	CEvent();

	CEvent(CEvent* evt);

	CEvent(int fromClient, int toClient, BYTE audience, BYTE type);
	CEvent(int fromClient, int toClient, BYTE audience, BYTE type, BYTE data, int length);
	CEvent(int fromClient, int toClient, BYTE audience, BYTE type, BYTE* data, int length);
	CEvent(int fromClient, int toClient, BYTE audience, BYTE type, const char* data, int length);
	CEvent(int fromClient, int toClient, BYTE audience, BYTE type, void* data, int length, void* playerInfo);

	~CEvent();


	void Clear();
};
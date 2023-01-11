#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CEvent.h"

class CClient
{
public:

	enum
	{
		CS_NOTCONNECTED = 0,
		CS_CONNECTING,
		CS_CONNECTED,
		
		CS_MAX,

		HOSTNAME_LENGTH = 128
	};

	CGlobalObjects* m_globalObjects;

	CEvent* m_event;

	BYTE m_connectionState;

	SOCKET m_connectSocket;

	int m_clientNbr;

	char m_ip[15];

	char m_port[6];

	char m_hostname[CClient::HOSTNAME_LENGTH];

	bool m_isRunning;

	HANDLE m_hThread;

	UINT m_receiveThreadId;


	CClient();

	CClient(CGlobalObjects* globalObjects, CEvent* evt);

	~CClient();


	bool ConnectSocket(addrinfo* ptr);

	void InitializeWinsock();

	void CreateSocket(CEvent* evt);

	void SendEvent(CEvent* evt);

	void Disconnect();

	void ClientReceive();
};
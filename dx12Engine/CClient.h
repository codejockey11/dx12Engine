#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CNetwork.h"

class CClient
{
public:

	enum ClientState
	{
		E_CONNECTED = 0,
		E_CONNECTING,
		E_NOTCONNECTED
	};

	enum
	{
		E_HOSTNAME_LENGTH = 64
	};

	BYTE m_connectionState;
	
	CGlobalObjects* m_globalObjects;
	CNetwork* m_network;
	
	HANDLE m_hThread;
	SOCKET m_socket;
	UINT m_receiveThreadId;
	
	bool m_isRunning;
	
	char m_hostname[CClient::E_HOSTNAME_LENGTH];

	CClient();
	CClient(CGlobalObjects* globalObjects, const char* address);
	~CClient();

	bool ConnectSocket(addrinfo* ptr);
	void CreateSocket(const char* address);
	void InitializeWinsock();
	void Disconnect();
	void Receive();
	void Send(CNetwork* network);

private:

	char m_ip[15];
	char m_port[6];

	WSADATA	wsaData;
};
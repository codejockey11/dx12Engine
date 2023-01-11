#include "CClient.h"

CClient::CClient()
{
	memset(this, 0x00, sizeof(CClient));
}

CClient::CClient(CGlobalObjects* globalObjects, CEvent* evt)
{
	memset(this, 0x00, sizeof(CClient));

	m_globalObjects = globalObjects;

	m_event = new CEvent();

	CClient::InitializeWinsock();

	CClient::CreateSocket(evt);
}

CClient::~CClient()
{
	delete m_event;
}

void CClient::InitializeWinsock()
{
	WORD wVersionRequested = MAKEWORD(2, 2);

	WSADATA	wsaData;

	int err = WSAStartup(wVersionRequested, &wsaData);

	if (err != 0)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::InitializeWinsock::WSAStartup:%i\n", err);

		return;
	}

	m_globalObjects->m_errorLog->WriteError("CClient::InitializeWinsock::WSAStartup:%s\n", wsaData.szDescription);
}

void CClient::CreateSocket(CEvent* evt)
{
	if (evt->m_length > 0)
	{
		size_t i = strcspn((char*)evt->m_data, ":");

		memcpy_s((void*)m_ip, 15, (void*)evt->m_data, i);

		i++;

		int c = 0;

		for (size_t x = i; x < i + 5; x++)
		{
			memcpy((void*)&m_port[c], (void*)&evt->m_data[x], 1);

			c++;
		}
	}
	else
	{
		memcpy_s((void*)m_ip, 15, (void*)"127.0.0.1", 9);
		memcpy((void*)m_port, (void*)"26105", 5);
	}

	struct addrinfo* result = {};
	struct addrinfo* ptr = {};
	struct addrinfo  hints = {};

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO::IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	size_t err = getaddrinfo(m_ip, m_port, &hints, &result);

	if (err != 0)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::CreateSocket::getaddrinfo:%i\n", err);

		return;
	}

	ptr = result;

	while (ptr != NULL)
	{
		m_connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (m_connectSocket == INVALID_SOCKET)
		{
			m_globalObjects->m_errorLog->WriteError("CClient::CreateSocket::socket:%s\n", m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());

			return;
		}

		if (CClient::ConnectSocket(ptr))
		{
			m_connectionState = CClient::CS_CONNECTING;

			break;
		}

		ptr = ptr->ai_next;
	}

	freeaddrinfo(result);

	gethostname(m_hostname, CClient::HOSTNAME_LENGTH);

	m_globalObjects->m_errorLog->WriteError("CClient::CreateSocket::hostname:%s\n", m_hostname);
}

bool CClient::ConnectSocket(addrinfo* ptr)
{
	size_t err = connect(m_connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

	if (err == SOCKET_ERROR)
	{
		closesocket(m_connectSocket);

		m_connectSocket = 0;

		m_globalObjects->m_errorLog->WriteError("CClient::ConnectSocket::Unable to connect to server\n");

		return false;
	}

	return true;
}

void CClient::SendEvent(CEvent* evt)
{
	if (m_connectionState != CClient::CS_CONNECTED)
	{
		return;
	}

	int totalBytes = send(m_connectSocket, (char*)evt, sizeof(CEvent), 0);

	if (totalBytes == 0)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::SendEvent::send:Server Closing Connection\n");

		m_connectionState = CClient::CS_NOTCONNECTED;
	}
	else if (totalBytes == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::SendEvent::send:%s\n", m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());
	}
}

void CClient::Disconnect()
{
	if (m_connectionState != CClient::CS_CONNECTED)
	{
		return;
	}

	size_t err = shutdown(m_connectSocket, SD_BOTH);

	if (err == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::Disconnect::shutdown:%s\n", m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());
	}

	m_connectionState = CClient::CS_NOTCONNECTED;


	m_isRunning = false;

	WaitForSingleObject(m_hThread, 3000);

	CloseHandle(m_hThread);

	
	closesocket(m_connectSocket);

	m_connectSocket = 0;

	WSACleanup();
}

void CClient::ClientReceive()
{
	int totalBytes = recv(m_connectSocket, (char*)m_event, sizeof(CEvent), 0);

	if (totalBytes == 0)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::ClientReceive::Server Closing Connection\n");

		m_connectionState = CClient::CS_NOTCONNECTED;
	}
	// WSAEINTR for some reason the hardware stopped the function call so skipping the error check 
	else if ((totalBytes == SOCKET_ERROR) && (WSAGetLastError() != WSAEINTR))
	{
		m_globalObjects->m_errorLog->WriteError("CClient::ClientReceive::recv:%s\n", m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText());
	}
}
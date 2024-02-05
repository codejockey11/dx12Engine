#include "CClient.h"

/*
*/
CClient::CClient()
{
	memset(this, 0x00, sizeof(CClient));
}

/*
*/
CClient::CClient(CGlobalObjects* globalObjects, const char* address)
{
	memset(this, 0x00, sizeof(CClient));

	m_globalObjects = globalObjects;

	m_network = new CNetwork();

	CClient::InitializeWinsock();
	CClient::CreateSocket(address);
}

/*
*/
CClient::~CClient()
{
	CClient::Disconnect();

	delete m_network;
}

/*
*/
bool CClient::ConnectSocket(addrinfo* ptr)
{
	size_t err = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);

	if (err == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::ConnectSocket::%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());

		return false;
	}

	return true;
}

/*
*/
void CClient::CreateSocket(const char* address)
{
	if (address[0] != 0x00)
	{
		size_t i = strcspn(address, ":");

		// i is length of ip address
		memcpy_s((void*)m_ip, 15, (void*)address, i);

		// skip the :
		i++;

		// copy the port/remaining bytes to m_port
		memcpy_s((void*)m_port, 6, (void*)&address[i], strlen((char*)&address[i]));
	}
	else
	{
		memcpy_s((void*)m_ip, 15, (void*)"127.0.0.1", 9);
		memcpy_s((void*)m_port, 6, (void*)"26105", 5);
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
		m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (m_socket == INVALID_SOCKET)
		{
			m_globalObjects->m_errorLog->WriteError("CClient::CreateSocket::socket:%s %s\n",
				m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
				m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());

			return;
		}

		if (CClient::ConnectSocket(ptr))
		{
			m_connectionState = CClient::ClientState::E_CONNECTING;

			break;
		}

		ptr = ptr->ai_next;
	}

	freeaddrinfo(result);

	gethostname(m_hostname, CClient::E_HOSTNAME_LENGTH);

	m_globalObjects->m_errorLog->WriteError("CClient::CreateSocket::hostname:%s\n", m_hostname);
}

/*
*/
void CClient::Disconnect()
{
	if (m_connectionState != CClient::ClientState::E_CONNECTED)
	{
		return;
	}

	m_connectionState = CClient::ClientState::E_NOTCONNECTED;

	m_isRunning = false;


	int err = shutdown(m_socket, SD_BOTH);
	
	WaitForSingleObject(m_hThread, 50);

	CloseHandle(m_hThread);

	if (err == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::Disconnect::shutdown:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());
	}

	m_socket = 0;

	WSACleanup();
}

/*
*/
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

/*
*/
void CClient::Receive()
{
	int totalBytes = recv(m_socket, (char*)m_network, sizeof(CNetwork), 0);

	if (totalBytes == 0)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::Receive::Server Closing Connection\n");

		m_connectionState = CClient::ClientState::E_NOTCONNECTED;

		m_isRunning = false;
	}
	else if (totalBytes == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::Receive::recv:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());

		m_connectionState = CClient::ClientState::E_NOTCONNECTED;

		m_isRunning = false;
	}
}

/*
*/
void CClient::Send(CNetwork* network)
{
	if (m_connectionState != CClient::ClientState::E_CONNECTED)
	{
		return;
	}

	int totalBytes = send(m_socket, (char*)network, sizeof(CNetwork), 0);

	if (totalBytes == 0)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::Send::send:Server Closing Connection\n");

		m_connectionState = CClient::ClientState::E_NOTCONNECTED;
	}
	else if (totalBytes == SOCKET_ERROR)
	{
		m_globalObjects->m_errorLog->WriteError("CClient::Send::send:%s %s\n",
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_name->GetText(),
			m_globalObjects->m_errorLog->GetWinsockErrorMessage(WSAGetLastError())->m_msg->GetText());

		m_connectionState = CClient::ClientState::E_NOTCONNECTED;
	}
}
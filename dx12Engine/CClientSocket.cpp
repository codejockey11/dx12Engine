#include "CClientSocket.h"

CClientSocket::CClientSocket()
{
	memset(this, 0x00, sizeof(CClientSocket));
}

CClientSocket::CClientSocket(UINT clientNumber, SOCKET socket)
{
	memset(this, 0x00, sizeof(CClientSocket));

	m_clientNbr = clientNumber;
	m_socket = socket;
}

CClientSocket::~CClientSocket()
{
}
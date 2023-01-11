#pragma once

#include "standard.h"

class CClientSocket
{
public:

	UINT m_clientNbr;

	SOCKET m_socket;


	CClientSocket();

	CClientSocket(UINT clientNumber, SOCKET socket);

	~CClientSocket();
};
#include "CNetwork.h"

/*
*/
CNetwork::CNetwork()
{
	memset(this, 0x00, sizeof(CNetwork));
}

/*
*/
CNetwork::CNetwork(BYTE audience, BYTE type, void* data, int length, void* playerInfo, void* serverInfo)
{
	memset(this, 0x00, sizeof(CNetwork));

	m_audience = audience;
	m_type = type;

	CNetwork::SetData(data, length);
	CNetwork::SetPlayerInfo(playerInfo);
	CNetwork::SetServerInfo(serverInfo);
}

/*
*/
CNetwork::~CNetwork()
{
}

/*
*/
void CNetwork::SetData(void* data, int length)
{
	if (data == nullptr)
	{
		return;
	}

	if (length > CNetwork::E_DATA_SIZE)
	{
		length = CNetwork::E_DATA_SIZE;
	}

	memcpy_s((void*)m_data, CNetwork::E_DATA_SIZE, data, length);
}

/*
*/
void CNetwork::SetPlayerInfo(void* playerInfo)
{
	if (playerInfo == nullptr)
	{
		return;
	}

	memcpy_s((void*)m_playerInfo, CNetwork::E_PLAYER_INFO_SIZE, playerInfo, CNetwork::E_PLAYER_INFO_SIZE);
}

/*
*/
void CNetwork::SetServerInfo(void* serverInfo)
{
	if (serverInfo == nullptr)
	{
		return;
	}

	memcpy_s((void*)m_serverInfo, CNetwork::E_SERVER_INFO_SIZE, serverInfo, CNetwork::E_SERVER_INFO_SIZE);
}
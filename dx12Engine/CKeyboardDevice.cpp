#include "CKeyboardDevice.h"

/*
*/
CKeyboardDevice::CKeyboardDevice()
{
	memset(this, 0x00, sizeof(CKeyboardDevice));

	m_keyMap = new CKeyMap[CKeyboardDevice::E_MAX_KEYS]();

	CKeyboardDevice::SetKeyMap(VK_F11, CKeyboardDevice::E_KR_ONCE,
		CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_TOGGLE_CONSOLE, 0, 0);

	CKeyboardDevice::SetKeyMap(CKeyboardDevice::E_VK_EQUALS, CKeyboardDevice::E_KR_ONCE,
		CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_TOGGLE_WINDOW, 0, 0);

	CKeyboardDevice::SetKeyMap(VK_ESCAPE, CKeyboardDevice::E_KR_ONCE,
		CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_EXIT_GAME, 0, 0);

	CKeyboardDevice::SetKeyMap(CKeyboardDevice::E_VK_W, CKeyboardDevice::E_KR_REPEATS,
		CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_MOVE, CNetwork::ClientEvent::E_CE_PLAYER_FORWARD, 1);
	
	CKeyboardDevice::SetKeyMap(CKeyboardDevice::E_VK_S, CKeyboardDevice::E_KR_REPEATS,
		CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_MOVE, CNetwork::ClientEvent::E_CE_PLAYER_BACKWARD, 1);
	
	CKeyboardDevice::SetKeyMap(CKeyboardDevice::E_VK_A, CKeyboardDevice::E_KR_REPEATS,
		CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_MOVE, CNetwork::ClientEvent::E_CE_PLAYER_STRAFE_LEFT, 1);
	
	CKeyboardDevice::SetKeyMap(CKeyboardDevice::E_VK_D, CKeyboardDevice::E_KR_REPEATS,
		CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_MOVE, CNetwork::ClientEvent::E_CE_PLAYER_STRAFE_RIGHT, 1);
	
	CKeyboardDevice::SetKeyMap(VK_SPACE, CKeyboardDevice::E_KR_ONCE,
		CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_MOVE, CNetwork::ClientEvent::E_CE_PLAYER_ATTACK, 1);
}

/*
*/
CKeyboardDevice::~CKeyboardDevice()
{
	delete[] m_keyMap;
}

/*
*/
void CKeyboardDevice::GetState()
{
	for (UINT i = 0; i < CKeyboardDevice::E_MAX_KEYS; i++)
	{
		if (m_keys[i])
		{
			m_keyMap[i].m_count++;

			if (m_keyMap[i].m_count > 2)
			{
				m_keyMap[i].m_count = 2;
			}
		}
		else
		{
			m_keyMap[i].m_count = 0;
		}
	}
}

/*
*/
void CKeyboardDevice::KeyDown(int key)
{
	m_keys[key] = 1;
}

/*
*/
void CKeyboardDevice::KeyUp(int key)
{
	m_keys[key] = 0;
}

/*
*/
void CKeyboardDevice::SetKeyMap(UINT key, BYTE repeats, BYTE audience, BYTE type, BYTE data, int length)
{
	m_keyMap[key].m_count = 0;
	m_keyMap[key].m_repeats = repeats;

	m_keyMap[key].m_network = new CNetwork(audience, type, (void*)&data, length, nullptr, nullptr);
}
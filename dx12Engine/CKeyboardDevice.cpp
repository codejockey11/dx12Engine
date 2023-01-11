#include "CKeyboardDevice.h"

CKeyboardDevice::CKeyboardDevice()
{
	memset(this, 0x00, sizeof(CKeyboardDevice));

	m_keyMap = new CKeyMap[CKeyboardDevice::KEY_COUNT]();

	CKeyboardDevice::SetKeyMap(VK_F11, CKeyboardDevice::KR_ONESHOT, CEvent::ET_LOCAL, CEvent::ET_TOGGLECONSOLE, 0, 0);

	CKeyboardDevice::SetKeyMap(CKeyboardDevice::VK_EQUALS, CKeyboardDevice::KR_ONESHOT, CEvent::ET_LOCAL, CEvent::ET_WINDOWMODE, 0, 0);

	CKeyboardDevice::SetKeyMap(VK_ESCAPE, CKeyboardDevice::KR_ONESHOT, CEvent::ET_LOCAL, CEvent::ET_EXIT, 0, 0);

	CKeyboardDevice::SetKeyMap(CKeyboardDevice::VK_W, CKeyboardDevice::KR_REPEATER, CEvent::ET_SERVER, CEvent::CServerEvent::SE_PLAYERMOVE, CEvent::ET_FORWARD, 1);
	CKeyboardDevice::SetKeyMap(CKeyboardDevice::VK_S, CKeyboardDevice::KR_REPEATER, CEvent::ET_SERVER, CEvent::CServerEvent::SE_PLAYERMOVE, CEvent::ET_BACKWARD, 1);
	CKeyboardDevice::SetKeyMap(CKeyboardDevice::VK_A, CKeyboardDevice::KR_REPEATER, CEvent::ET_SERVER, CEvent::CServerEvent::SE_PLAYERMOVE, CEvent::ET_STRAFELEFT, 1);
	CKeyboardDevice::SetKeyMap(CKeyboardDevice::VK_D, CKeyboardDevice::KR_REPEATER, CEvent::ET_SERVER, CEvent::CServerEvent::SE_PLAYERMOVE, CEvent::ET_STRAFERIGHT, 1);

	CKeyboardDevice::SetKeyMap(VK_SPACE, CKeyboardDevice::KR_ONESHOT, CEvent::ET_SERVER, CEvent::ET_ATTACK, 0, 0);
}

CKeyboardDevice::~CKeyboardDevice()
{
	delete[] m_keyMap;
}

void CKeyboardDevice::GetState()
{
	for (UINT k = 0; k < CKeyboardDevice::KEY_COUNT; k++)
	{
		if (m_keys[k])
		{
			m_keyMap[k].m_count++;
		}
		else
		{
			m_keyMap[k].m_count = 0;
		}
	}
}

void CKeyboardDevice::SetKeyMap(UINT number, BYTE repeater, BYTE audience, BYTE type, BYTE data, int length)
{
	m_keyMap[number].m_count = 0;
	m_keyMap[number].m_repeater = repeater;

	m_keyMap[number].m_event = new CEvent(0, 0, audience, type, data, length);
}

void CKeyboardDevice::KeyDown(int key)
{
	m_keys[key] = 1;
}

void CKeyboardDevice::KeyUp(int key)
{
	m_keys[key] = 0;
}
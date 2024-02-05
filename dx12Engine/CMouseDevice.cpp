#include "CMouseDevice.h"

/*
*/
CMouseDevice::CMouseDevice()
{
	memset(this, 0x00, sizeof(CMouseDevice));
}

/*
*/
CMouseDevice::CMouseDevice(CErrorLog* errorLog, CWindow* window, float sensitivity)
{
	memset(this, 0x00, sizeof(CMouseDevice));

	m_errorLog = errorLog;

	m_window = window;

	m_sensitivity = sensitivity;

	GetCursorPos(&m_physicalPosition);

	m_prevPosition.x = m_physicalPosition.x;
	m_prevPosition.y = m_physicalPosition.y;

	m_position.p.x = (float)m_physicalPosition.x;
	m_position.p.y = (float)m_physicalPosition.y;

	m_positionSaved = false;

	m_keyMap = new CKeyMap[CMouseDevice::E_BUTTON_COUNT]();

	CMouseDevice::SetKeyMap(CMouseDevice::E_LMB, CMouseDevice::E_KR_REPEATS,
		CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_MOVE, CNetwork::ClientEvent::E_CE_PLAYER_ATTACK, 1);

	CMouseDevice::SetKeyMap(CMouseDevice::E_MMB, CMouseDevice::E_KR_REPEATS,
		CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_MOVE, CNetwork::ClientEvent::E_CE_PLAYER_STOP, 1);

	CMouseDevice::SetKeyMap(CMouseDevice::E_RMB, CMouseDevice::E_KR_REPEATS,
		CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_PLAYER_FIRST_PERSON_MOVE, 0, 1);
}

/*
*/
CMouseDevice::~CMouseDevice()
{
	delete[] m_keyMap;
}

/*
*/
void CMouseDevice::FirstPersonReset()
{
	// resetting mouse move items
	m_lX = 0;
	m_lY = 0;
	m_lZ = 0;

	SetCursorPos(m_window->m_middle.x, m_window->m_middle.y);

	m_prevPosition.x = m_window->m_middle.x;
	m_prevPosition.y = m_window->m_middle.y;
}

/*
*/
void CMouseDevice::GetState()
{
	// Logic for mouse location to screen resolution
	GetCursorPos(&m_physicalPosition);

	m_position.p.x = (float)m_physicalPosition.x * ((float)m_window->m_width / (float)m_window->m_desktop.right);
	m_position.p.y = (float)m_physicalPosition.y * ((float)m_window->m_height / (float)m_window->m_desktop.bottom);

	if (m_window->m_windowed)
	{
		ScreenToClient(m_window->m_hWnd, &m_physicalPosition);

		m_position.p.x = (float)m_physicalPosition.x;// + 8.0f;  border compensate
		m_position.p.y = (float)m_physicalPosition.y;// + 32.0f;  title bar compensate
	}

	// Setting keymap for events
	for (UINT i = 0; i < CMouseDevice::E_BUTTON_COUNT; i++)
	{
		if (m_rgbButtons[i])
		{
			m_keyMap[i].m_count++;
		}
		else
		{
			m_keyMap[i].m_count = 0;
		}
	}
}

/*
*/
void CMouseDevice::MouseClick(int button, int value)
{
	m_rgbButtons[button] = value;
}

/*
*/
void CMouseDevice::MouseMove()
{
	GetCursorPos(&m_physicalPosition);

	m_lX = m_physicalPosition.x - m_prevPosition.x;
	m_lY = m_physicalPosition.y - m_prevPosition.y;
}
/*
*/
void CMouseDevice::MouseWheel(long value)
{
	m_lZ = value;
}

/*
*/
void CMouseDevice::SaveLastPosition()
{
	if (m_positionSaved)
	{
		return;
	}

	GetCursorPos(&m_savePosition);

	m_positionSaved = true;

	ShowCursor(false);
}

/*
*/
void CMouseDevice::SetKeyMap(UINT number, BYTE repeats, BYTE audience, BYTE type, BYTE data, int length)
{
	m_keyMap[number].m_count = 0;
	m_keyMap[number].m_repeats = repeats;

	m_keyMap[number].m_network = new CNetwork(audience, type, (void*)&data, length, nullptr, nullptr);
}

/*
*/
void CMouseDevice::SetLastPosition()
{
	SetCursorPos(m_savePosition.x, m_savePosition.y);

	m_positionSaved = false;

	ShowCursor(true);
}
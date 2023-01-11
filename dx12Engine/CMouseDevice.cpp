#include "CMouseDevice.h"

CMouseDevice::CMouseDevice()
{
	memset(this, 0x00, sizeof(CMouseDevice));
}

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

	m_keyMap = new CKeyMap[CMouseDevice::BUTTON_COUNT]();

	CMouseDevice::SetKeyMap(CMouseDevice::LMB, CMouseDevice::KR_ONESHOT, CEvent::ET_SERVER, CEvent::ET_ATTACK, 0, 1);

	CMouseDevice::SetKeyMap(CMouseDevice::MMB, CMouseDevice::KR_ONESHOT, CEvent::ET_LOCAL, CEvent::ET_EMPTY, 0, 1);

	CMouseDevice::SetKeyMap(CMouseDevice::RMB, CMouseDevice::KR_REPEATER, CEvent::ET_SERVER, CEvent::ET_FPSMOVE, 0, 1);
}

CMouseDevice::~CMouseDevice()
{
	delete[] m_keyMap;
}

void CMouseDevice::GetState()
{
	// logic for mouse location to screen resolution
	GetCursorPos(&m_physicalPosition);

	m_position.p.x = (float)m_physicalPosition.x * ((float)m_window->m_width / (float)m_window->m_desktop.right);
	m_position.p.y = (float)m_physicalPosition.y * ((float)m_window->m_height / (float)m_window->m_desktop.bottom);

	if (m_window->m_windowed)
	{
		ScreenToClient(m_window->m_hWnd, &m_physicalPosition);

		m_position.p.x = (float)m_physicalPosition.x;// + 8.0f;  border compensate
		m_position.p.y = (float)m_physicalPosition.y;// + 32.0f;  title bar compensate
	}

	// setting keymap for events
	for (UINT k = 0; k < CMouseDevice::BUTTON_COUNT; k++)
	{
		if (m_rgbButtons[k])
		{
			m_keyMap[k].m_count++;
		}
		else
		{
			m_keyMap[k].m_count = 0;
		}
	}
}

void CMouseDevice::SetKeyMap(UINT number, BYTE repeater, BYTE audience, BYTE type, BYTE data, int length)
{
	m_keyMap[number].m_count = 0;
	m_keyMap[number].m_repeater = repeater;

	m_keyMap[number].m_event = new CEvent(0, 0, audience, type, data, length);
}

void CMouseDevice::MouseMove()
{
	GetCursorPos(&m_physicalPosition);

	m_lX = m_physicalPosition.x - m_prevPosition.x;
	m_lY = m_physicalPosition.y - m_prevPosition.y;
}

void CMouseDevice::MouseClick(int button, int value)
{
	m_rgbButtons[button] = value;
}

void CMouseDevice::MouseWheel(long value)
{
	m_lZ = value;
}

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

void CMouseDevice::SetLastPosition()
{
	SetCursorPos(m_savePosition.x, m_savePosition.y);

	m_positionSaved = false;

	ShowCursor(true);
}
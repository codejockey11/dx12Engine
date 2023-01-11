#include "CWindowsButton.h"

CWindowsButton::CWindowsButton()
{
	memset(this, 0x00, sizeof(CWindowsButton));
}

CWindowsButton::CWindowsButton(CGlobalObjects* globalObjects, HMENU hMenu, const char* name, CVertex2 size, CVertex2 position)
{
	memset(this, 0x00, sizeof(CWindowsButton));

	m_globalObjects = globalObjects;

	m_position.x = (int)position.p.x;
	m_position.y = (int)position.p.y;

	m_size.x = (int)size.p.x;
	m_size.y = (int)size.p.y;

	m_button = CreateWindow(WC_BUTTON, name, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		m_position.x, m_position.y,
		m_size.x, m_size.y,
		m_globalObjects->m_window->m_hWnd,
		hMenu,
		m_globalObjects->m_window->m_hInstance, NULL);

	// Window mode to full screen does not accomodate screen resolution.
	m_fullscreenRatio.p.x = (float)m_globalObjects->m_window->m_desktop.right / (float)m_globalObjects->m_window->m_width;
	m_fullscreenRatio.p.y = (float)m_globalObjects->m_window->m_desktop.bottom / (float)m_globalObjects->m_window->m_height;

	m_fullscreenPosition.x = (int)(m_position.x * m_fullscreenRatio.p.x);
	m_fullscreenPosition.y = (int)(m_position.y * m_fullscreenRatio.p.y);

	m_fullscreenSize.x = (int)(m_size.x * m_fullscreenRatio.p.x);
	m_fullscreenSize.y = (int)(m_size.y * m_fullscreenRatio.p.y);
}

CWindowsButton::~CWindowsButton()
{
	// When a window is destroyed, all its child windows are also destroyed.
	// No need to cleanup Windows button objects
	// delete button;
}

void CWindowsButton::FullscreenWindowsButton()
{
	SetWindowPos(m_button, 0, m_fullscreenPosition.x, m_fullscreenPosition.y, m_fullscreenSize.x, m_fullscreenSize.y, SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWindowsButton::WindowWindowsButton()
{
	SetWindowPos(m_button, 0, m_position.x, m_position.y, m_size.x, m_size.y, SWP_NOACTIVATE | SWP_NOZORDER);
}
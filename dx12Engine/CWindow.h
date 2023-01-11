#pragma once

#include "standard.h"

#include "Resource.h"

#include "CWebBrowser.h"

class CWindow
{
public:

	enum
	{
		WT_MAIN = 0,
		WT_BROWSER,
		BTN_BROWSER = 1000,
		BTN_BACK,
		BTN_FORWARD,
		BTN_REFRESH,
		BTN_GO,
		BTN_CLOSE,
	};

	UINT m_posX;
	UINT m_posY;

	UINT m_width;
	UINT m_height;

	RECT m_desktop;

	HWND m_hWnd;
	HWND m_hWndBrowser;
	HWND m_hWndAddressBar;

	CWebBrowser* m_browser;

	HINSTANCE m_hInstance;

	bool m_windowed;

	bool m_visible;

	POINT m_middle;

	RECT m_windowRect;

	RECT m_clientRect;

	POINT m_clientToScreen;

	float m_dpi;

	char* m_className;


	CWindow();

	CWindow(HWND hWnd, HINSTANCE hInstance, bool windowed, BYTE type, UINT width, UINT height, const char* className, const char* title, WNDPROC wndProc);

	~CWindow();


	RECT GetWindowRectangle();

	RECT GetClientRectangle();

	POINT GetClientToScreen();

	float GetDpi();
};
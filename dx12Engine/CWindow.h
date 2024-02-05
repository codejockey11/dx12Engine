#pragma once

#include "standard.h"

#include "Resource.h"

#include "CWebBrowser.h"

class CWindow
{
public:

	enum
	{
		E_WT_MAIN = 0,
		E_WT_BROWSER,
		E_BTN_BROWSER = 1000,
		E_BTN_BACK,
		E_BTN_FORWARD,
		E_BTN_REFRESH,
		E_BTN_GO,
		E_BTN_CLOSE,
	};

	CWebBrowser* m_browser;
	
	HINSTANCE m_hInstance;
	
	HWND m_hWnd;
	HWND m_hWndAddressBar;
	HWND m_hWndBrowser;
	
	POINT m_clientToScreen;
	POINT m_middle;
	
	RECT m_clientRect;
	RECT m_desktop;
	RECT m_windowRect;
	
	UINT m_height;
	UINT m_posX;
	UINT m_posY;
	UINT m_width;
	
	bool m_visible;
	bool m_windowed;
	
	char* m_className;
	
	float m_dpi;

	CWindow();
	CWindow(HWND hWnd, HINSTANCE hInstance, bool windowed, BYTE type, UINT width, UINT height, const char* className, const char* title, WNDPROC wndProc);
	~CWindow();

	RECT GetClientRectangle();
	POINT GetClientToScreen();
	float GetDpi();
	RECT GetWindowRectangle();
};
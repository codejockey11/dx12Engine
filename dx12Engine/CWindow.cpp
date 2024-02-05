#include "CWindow.h"

/*
*/
CWindow::CWindow()
{
	memset(this, 0x00, sizeof(CWindow));
}

/*
*/
CWindow::CWindow(HWND hWnd, HINSTANCE hInstance, bool windowed, BYTE type, UINT width, UINT height, const char* className, const char* title, WNDPROC wndProc)
{
	memset(this, 0x00, sizeof(CWindow));

	m_hInstance = hInstance;

	m_windowed = windowed;

	m_width = width;
	m_height = height;

	m_className = (char*)className;

	m_posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	m_posY = ((GetSystemMetrics(SM_CYSCREEN) - height) / 2) - 20;


	GetWindowRect(GetDesktopWindow(), &m_desktop);

	m_middle.x = m_posX + (m_width / 2);
	m_middle.y = m_posY + (m_height / 2);


	WNDCLASSEX wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DX12ENGINE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//wcex.hCursor = LoadCursorFromFileA("dx12Engine.ico");
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	//wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DX12ENGINE);
	wcex.lpszClassName = m_className;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassEx(&wcex);


	switch (type)
	{
		case CWindow::E_WT_MAIN:
		{
			if (m_windowed)
			{
				// anything other than popup will require YOU to handle mouse positions relative to title bar and borders
				m_hWnd = CreateWindowEx(0, m_className, title, WS_POPUP, m_posX, m_posY, m_width, m_height, nullptr, nullptr, m_hInstance, nullptr);
			}
			else
			{
				m_hWnd = CreateWindowEx(0, m_className, title, WS_POPUP, 0, 0, m_width, m_height, nullptr, nullptr, m_hInstance, nullptr);
			}

			break;
		}
		case CWindow::E_WT_BROWSER:
		{
			m_hWndBrowser = CreateWindowEx(0, m_className, title, WS_POPUP | WS_BORDER, m_posX, m_posY, m_width, m_height, hWnd, nullptr, m_hInstance, nullptr);

			HWND hWndBack = CreateWindowEx(0, _T("BUTTON"), _T("<"), WS_CHILD | WS_VISIBLE | BS_PUSHBOX | BS_BITMAP,
				5, 5,
				80, 30,
				m_hWndBrowser, (HMENU)CWindow::E_BTN_BACK, m_hInstance, NULL);

			HWND hWndForward = CreateWindowEx(0, _T("BUTTON"), _T(">"), WS_CHILD | WS_VISIBLE | BS_PUSHBOX | BS_BITMAP,
				90, 5,
				80, 30,
				m_hWndBrowser, (HMENU)CWindow::E_BTN_FORWARD, m_hInstance, NULL);

			HWND hWndRefresh = CreateWindowEx(0, _T("BUTTON"), _T("Refresh"), WS_CHILD | WS_VISIBLE | BS_PUSHBOX | BS_BITMAP,
				175, 5,
				80, 30,
				m_hWndBrowser, (HMENU)CWindow::E_BTN_REFRESH, m_hInstance, NULL);

			m_hWndAddressBar = CreateWindowEx(0, _T("EDIT"), _T("http://codeproject.com/"), WS_CHILD | WS_VISIBLE | WS_BORDER,
				260, 10,
				200, 20,
				m_hWndBrowser, NULL, m_hInstance, NULL);

			HWND hWndGo = CreateWindowEx(0, _T("BUTTON"), _T("Go"), WS_CHILD | WS_VISIBLE | BS_PUSHBOX | BS_BITMAP,
				465, 5,
				80, 30,
				m_hWndBrowser, (HMENU)CWindow::E_BTN_GO, m_hInstance, NULL);

			HWND hWndClose = CreateWindowEx(0, _T("BUTTON"), _T("Close"), WS_CHILD | WS_VISIBLE | BS_PUSHBOX | BS_BITMAP,
				555, 5,
				80, 30,
				m_hWndBrowser, (HMENU)CWindow::E_BTN_CLOSE, m_hInstance, NULL);

			m_browser = new CWebBrowser(m_hWndBrowser, 20, 40, m_width - 20, m_height - 20);

			m_browser->Navigate("www.google.com");


			HBITMAP hBmp = LoadBitmap(m_hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

			SendMessage(hWndBack, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);
			SendMessage(hWndForward, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);
			SendMessage(hWndRefresh, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);
			SendMessage(hWndGo, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);
			SendMessage(hWndClose, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBmp);

			
			ShowWindow(m_hWndBrowser, SW_SHOWDEFAULT);

			SetWindowPos(m_hWndBrowser, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

			SetFocus(m_hWndBrowser);

			break;
		}
	}
}

/*
*/
CWindow::~CWindow()
{
	if (m_browser)
	{
		delete m_browser;
	}

	UnregisterClass(m_className, m_hInstance);
}

/*
*/
RECT CWindow::GetClientRectangle()
{
	GetClientRect(m_hWnd, &m_clientRect);

	return m_clientRect;
}

/*
*/
POINT CWindow::GetClientToScreen()
{
	ClientToScreen(m_hWnd, &m_clientToScreen);

	return m_clientToScreen;
}

/*
*/
float CWindow::GetDpi()
{
	m_dpi = (float)GetDpiForWindow(m_hWnd);

	return m_dpi;
}

/*
*/
RECT CWindow::GetWindowRectangle()
{
	GetWindowRect(m_hWnd, &m_windowRect);

	return m_windowRect;
}
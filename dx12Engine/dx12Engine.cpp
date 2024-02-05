#include "standard.h"

#include "CButtonManager.h"
#include "CCamera.h"
#include "CCommandListBundle.h"
#include "CConsole.h"
#include "CEventManager.h"
#include "CFrametime.h"
#include "CGlobalObjects.h"
#include "CTextField.h"
#include "CWindow.h"

CButton* m_buttons[CButton::E_MAX_BUTTONS];
CButtonManager* m_buttonManager;
CCamera* m_currentCamera;
CCommandListBundle* m_commandListBundle[CCommandListBundle::E_MAX_BUNDLES];
CConsole* m_console;
CD12Font* m_arial18;
CD12Font* m_consolas12;
CEventManager* m_eventManager;
CFrametime* m_frametime;
CGlobalObjects* m_globalObjects;
CTextField* m_password;
CTextField* m_userId;
CTexture* m_buttonImage;
CTexture* m_textImage;
CWindow* m_browser;

float bob = 0.0f;
float spin = 0.0f;
float count = 0.0f;

LRESULT CALLBACK BrowserWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

bool CheckButtonMouseOver();
bool CheckTextFieldMouseOver();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

void CheckWM_CHAR(CTextField* textField, WPARAM w);
void CheckWM_KEYDOWN(CTextField* textField, WPARAM w);
void Cleanup();
void GetControllerActivity();
void HandleKeyboard(WPARAM wParam);
void Initialize();
void LocalServerFrametime();
void RenderFrame();
void RenderText();

LRESULT CALLBACK BrowserWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case CWindow::E_BTN_BACK:
		{
			m_browser->m_browser->GoBack();

			break;
		}
		case CWindow::E_BTN_FORWARD:
		{
			m_browser->m_browser->GoForward();

			break;
		}
		case CWindow::E_BTN_REFRESH:
		{
			m_browser->m_browser->Refresh();

			break;
		}
		case CWindow::E_BTN_GO:
		{
			TCHAR* szUrl = new TCHAR[1024];

			GetWindowText(m_browser->m_hWndAddressBar, szUrl, 1024);

			m_browser->m_browser->Navigate(szUrl);

			break;
		}
		case CWindow::E_BTN_CLOSE:
		{
			DestroyWindow(hWnd);

			delete m_browser;

			m_browser = nullptr;

			break;
		}
		}

		return 0;

		break;
	}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		// Regular first person shooter mouse move
		/*
		case WM_MOUSEMOVE:
		{
			if (m_console->m_isVisible)
			{
				return 0;
			}

			if (m_browser != nullptr)
			{
				return 0;
			}

			if (m_currentCamera)
			{
				m_globalObjects->m_mouse->MouseMove();

				m_currentCamera->UpdateRotation((m_globalObjects->m_mouse->m_lY * m_globalObjects->m_mouse->m_sensitivity),
				(m_globalObjects->m_mouse->m_lX * m_globalObjects->m_mouse->m_sensitivity),
				(m_globalObjects->m_mouse->m_lZ * m_globalObjects->m_mouse->m_sensitivity));

				m_eventManager->m_playerInfo[m_eventManager->m_localPlayerNumber]->SetDirection(&m_currentCamera->m_look);

				m_globalObjects->m_mouse->FirstPersonReset();
			}

			return 0;
		}
		*/
		// MMO style mouse move
	case WM_RBUTTONDOWN:
	{
		if (m_console->m_isVisible)
		{
			return 0;
		}

		if (m_browser != nullptr)
		{
			return 0;
		}

		m_globalObjects->m_mouse->SaveLastPosition();

		m_globalObjects->m_mouse->FirstPersonReset();

		m_globalObjects->m_mouse->MouseClick(CMouseDevice::E_RMB, 1);

		return 0;

		break;
	}
	case WM_RBUTTONUP:
	{
		m_globalObjects->m_mouse->SetLastPosition();

		m_globalObjects->m_mouse->MouseClick(CMouseDevice::E_RMB, 0);

		return 0;

		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (m_console->m_isVisible)
		{
			return 0;
		}

		if (m_browser != nullptr)
		{
			return 0;
		}

		m_globalObjects->m_mouse->MouseClick(CMouseDevice::E_LMB, 1);

		return 0;

		break;
	}
	case WM_LBUTTONUP:
	{
		m_globalObjects->m_mouse->MouseClick(CMouseDevice::E_LMB, 0);

		return 0;

		break;
	}
	case WM_MBUTTONDOWN:
	{
		if (m_console->m_isVisible)
		{
			return 0;
		}

		if (m_browser != nullptr)
		{
			return 0;
		}

		m_globalObjects->m_mouse->MouseClick(CMouseDevice::E_MMB, 1);

		return 0;

		break;
	}
	case WM_MBUTTONUP:
	{
		m_globalObjects->m_mouse->MouseClick(CMouseDevice::E_MMB, 0);

		return 0;

		break;
	}
	case WM_MOUSEWHEEL:
	{
		long wheelDelta = (long)GET_WHEEL_DELTA_WPARAM(wParam);

		if (m_console->m_isVisible)
		{
			if (wheelDelta > 0)
			{
				m_console->Paging(CConsole::E_PAGE_BACKWARD);

				return 0;
			}
			if (wheelDelta < 0)
			{
				m_console->Paging(CConsole::E_PAGE_FORWARD);

				return 0;
			}
		}

		m_globalObjects->m_mouse->MouseWheel(wheelDelta);

		m_currentCamera->UpdateRotation((m_globalObjects->m_mouse->m_lY * m_globalObjects->m_mouse->m_sensitivity),
			(m_globalObjects->m_mouse->m_lX * m_globalObjects->m_mouse->m_sensitivity),
			(m_globalObjects->m_mouse->m_lZ * m_globalObjects->m_mouse->m_sensitivity));

		return 0;

		break;
	}
	case WM_CHAR:
	{
		if (m_console->m_isVisible)
		{
			if (wParam == VK_RETURN)
			{
				CNetwork* n = m_console->CommandFromConsole(m_eventManager->m_localPlayer);

				if (n != nullptr)
				{
					m_eventManager->Send(n);

					delete n;
				}

				return 0;
			}
			// ASCII values
			if ((wParam >= 32) && (wParam <= 126))
			{
				m_console->AddChar((char)wParam);

				return 0;
			}
		}

		if (m_userId->m_isActive)
		{
			CheckWM_CHAR(m_userId, wParam);
		}

		if (m_password->m_isActive)
		{
			CheckWM_CHAR(m_password, wParam);
		}

		return 0;

		break;
	}
	case WM_KEYDOWN:
	{
		if (m_console->m_isVisible)
		{
			switch (wParam)
			{
			case VK_PRIOR:
			{
				m_console->Paging(CConsole::E_PAGE_BACKWARD);

				return 0;

				break;
			}
			case VK_NEXT:
			{
				m_console->Paging(CConsole::E_PAGE_FORWARD);

				return 0;

				break;
			}
			case VK_BACK:
			{
				if (m_console->m_isVisible)
				{
					m_console->Backspace();
				}

				return 0;

				break;
			}
			case VK_RETURN:
			{
				return 0;

				break;
			}
			
			default:
			{
				// unshifted ASCII values
				if ((wParam >= 32) && (wParam <= 96))
				{
					return 0;
				}

				HandleKeyboard(wParam);

				return 0;

				break;
			}
			}
		}

		if (m_userId->m_isActive)
		{
			CheckWM_KEYDOWN(m_userId, wParam);

			return 0;
		}

		if (m_password->m_isActive)
		{
			CheckWM_KEYDOWN(m_password, wParam);

			return 0;
		}

		m_globalObjects->m_keyboard->KeyDown((int)wParam);

		return 0;

		break;
	}
	case WM_KEYUP:
	{
		m_globalObjects->m_keyboard->KeyUp((int)wParam);

		return 0;

		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);

		return 0;

		break;
	}
	case WM_COMMAND:
	{
		if (m_browser != nullptr)
		{
			return 0;
		}

		switch LOWORD(wParam)
		{
			// Adjust the buttons for the screen resolution
		case CButton::E_BTN_WINDOWMODE:
		{
			for (int i = 0; i < CButton::E_MAX_BUTTONS; i++)
			{
				if (m_globalObjects->m_window->m_windowed)
				{
					m_buttons[i]->m_windowsButton->FullScreenWindowsButton();
				}
				else
				{
					m_buttons[i]->m_windowsButton->WindowWindowsButton();
				}
			}

			break;
		}
		case CButton::E_BTN_BROWSER:
		{
			if (m_browser)
			{
				delete m_browser;
			}

			m_browser = new CWindow(m_globalObjects->m_window->m_hWnd,
				m_globalObjects->m_window->m_hInstance,
				true,
				CWindow::E_WT_BROWSER,
				m_globalObjects->m_window->m_width, m_globalObjects->m_window->m_height,
				"browserClass", "Browser",
				BrowserWindowProc);

			return 0;
		}
		}

		//m_buttons[(int)wParam - CButton::ControlsCount]->m_soundClicked->StartSound();

		m_buttons[(int)wParam - CButton::ControlsCount]->m_network->SetPlayerInfo(m_eventManager->m_localPlayer->m_playerInfo);
		m_buttons[(int)wParam - CButton::ControlsCount]->m_network->SetServerInfo(m_eventManager->m_localPlayer);

		// The order of this array must match the Controls enum in CButton
		m_eventManager->Send(m_buttons[(int)wParam - CButton::ControlsCount]->m_network);

		// Focus must be set to parent after button click
		SetFocus(m_globalObjects->m_window->m_hWnd);

		return 0;

		break;
	}
	case WM_CLOSE:
	{
		DestroyWindow(m_globalObjects->m_window->m_hWnd);

		return 0;

		break;
	}
	default:
	{
		break;
	}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool CheckButtonMouseOver()
{
	for (int i = 0; i < CButton::E_MAX_BUTTONS; i++)
	{
		if (m_buttons[i]->m_isMouseOver)
		{
			return m_buttons[i]->m_isMouseOver;
		}
	}

	return false;
}

bool CheckTextFieldMouseOver()
{
	if (m_userId->m_isMouseOver)
	{
		return true;
	}

	if (m_password->m_isMouseOver)
	{
		return true;
	}

	return false;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	HRESULT hr = OleInitialize(0);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	m_globalObjects = new CGlobalObjects(hInstance, "dx12EngineClass", "dx12Engine", MainWindowProc, CWindow::E_WT_MAIN, "log.txt", "config.txt");

	if (!m_globalObjects->m_window->m_hWnd)
	{
		delete m_globalObjects;

		return 0;
	}


	Initialize();


	MSG msg = {};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
		{
			// Everything is running on the local machine with no networking
			LocalServerFrametime();

			if (m_browser == nullptr)
			{
				RenderFrame();
			}

			GetControllerActivity();
		}
	}

	Cleanup();


	OleUninitialize();

	return (int)msg.wParam;
}

void CheckWM_CHAR(CTextField* textField, WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		CNetwork* n = textField->GetEvent();

		if (n != nullptr)
		{
			m_eventManager->Send(n);
		}

		textField->m_isActive = false;

		return;
	}
	// ASCII values
	if ((wParam >= 32) && (wParam <= 126))
	{
		textField->AddChar((char)wParam);

		return;
	}
}

void CheckWM_KEYDOWN(CTextField* textField, WPARAM wParam)
{
	switch (wParam)
	{
	case VK_BACK:
	{
		textField->Backspace();

		break;
	}
	// keydown is not ASCII
	default:
	{
		HandleKeyboard(wParam);

		// everything else happens with WM_CHAR

		break;
	}
	}
}

void Cleanup()
{
	// Wait for the GPU threads to complete
	m_globalObjects->m_videoDevice->WaitForGPU();

	if (m_browser)
	{
		delete m_browser;
	}

	delete m_buttonManager;
	delete m_password;
	delete m_userId;

	for (int i = 0; i < CCommandListBundle::E_MAX_BUNDLES; i++)
	{
		delete m_commandListBundle[i];
	}

	delete m_eventManager;
	delete m_console;
	delete m_frametime;
	delete m_globalObjects;
}

void GetControllerActivity()
{
	bool isActive = false;

	m_globalObjects->m_mouse->GetState();

	for (int k = 0; k < 4; k++)
	{
		for (int i = 0; i < CButton::E_MAX_BUTTONS; i++)
		{
			if ((m_buttons[i]->m_isMouseOver) && (m_globalObjects->m_mouse->m_keyMap[k].m_count == 1))
			{
				return;
			}
		}
	}

	if (m_userId->m_isMouseOver)
	{
		return;
	}

	if (m_password->m_isMouseOver)
	{
		return;
	}

	for (int k = 0; k < 4; k++)
	{
		if ((m_globalObjects->m_mouse->m_keyMap[k].m_count > 0) && (m_globalObjects->m_mouse->m_keyMap[k].m_repeats == CMouseDevice::E_KR_REPEATS))
		{
			if (m_globalObjects->m_mouse->m_keyMap[k].m_network != nullptr)
			{
				if (m_globalObjects->m_mouse->m_keyMap[k].m_network->m_type == CNetwork::ClientEvent::E_CE_PLAYER_FIRST_PERSON_MOVE)
				{
					if (m_currentCamera)
					{
						m_globalObjects->m_mouse->MouseMove();

						m_currentCamera->UpdateRotation((m_globalObjects->m_mouse->m_lY * m_globalObjects->m_mouse->m_sensitivity),
							(m_globalObjects->m_mouse->m_lX * m_globalObjects->m_mouse->m_sensitivity),
							(m_globalObjects->m_mouse->m_lZ * m_globalObjects->m_mouse->m_sensitivity));

						m_globalObjects->m_mouse->FirstPersonReset();
					}
				}
				else
				{
					isActive = true;

					if ((m_eventManager->m_localPlayer != nullptr) && (m_eventManager->m_localPlayer->m_isRunning))
					{
						m_globalObjects->m_mouse->m_keyMap[k].m_network->SetPlayerInfo((void*)m_eventManager->m_localPlayer->m_playerInfo);
						m_globalObjects->m_mouse->m_keyMap[k].m_network->SetServerInfo((void*)m_eventManager->m_localPlayer);
					}

					m_eventManager->Send(m_globalObjects->m_mouse->m_keyMap[k].m_network);

					if (m_globalObjects->m_mouse->m_keyMap[k].m_network->m_data[0] == CNetwork::ClientEvent::E_CE_PLAYER_STOP)
					{
						return;
					}
				}
			}
		}
		else if ((m_globalObjects->m_mouse->m_keyMap[k].m_count == 1) && (m_globalObjects->m_mouse->m_keyMap[k].m_repeats == CMouseDevice::E_KR_ONCE))
		{
			if (m_globalObjects->m_mouse->m_keyMap[k].m_network)
			{
				isActive = true;

				m_globalObjects->m_mouse->m_keyMap[k].m_network->SetPlayerInfo((void*)m_eventManager->m_localPlayer->m_playerInfo);
				m_globalObjects->m_mouse->m_keyMap[k].m_network->SetServerInfo((void*)m_eventManager->m_localPlayer);

				m_eventManager->Send(m_globalObjects->m_mouse->m_keyMap[k].m_network);
			}
		}
	}


	m_globalObjects->m_keyboard->GetState();

	for (int k = 0; k < 256; k++)
	{
		if ((m_globalObjects->m_keyboard->m_keyMap[k].m_count > 0) && (m_globalObjects->m_keyboard->m_keyMap[k].m_repeats == CKeyboardDevice::E_KR_REPEATS))
		{
			isActive = true;

			if ((m_eventManager->m_localPlayer != nullptr) && (m_eventManager->m_localPlayer->m_isRunning))
			{
				m_globalObjects->m_keyboard->m_keyMap[k].m_network->SetPlayerInfo((void*)m_eventManager->m_localPlayer->m_playerInfo);
				m_globalObjects->m_keyboard->m_keyMap[k].m_network->SetServerInfo((void*)m_eventManager->m_localPlayer);
			}

			m_eventManager->Send(m_globalObjects->m_keyboard->m_keyMap[k].m_network);
		}
		else if ((m_globalObjects->m_keyboard->m_keyMap[k].m_count == 1) && (m_globalObjects->m_keyboard->m_keyMap[k].m_repeats == CKeyboardDevice::E_KR_ONCE))
		{
			if (m_globalObjects->m_keyboard->m_keyMap[k].m_network)
			{
				isActive = true;

				if ((m_eventManager->m_localPlayer != nullptr) && (m_eventManager->m_localPlayer->m_isRunning))
				{
					m_globalObjects->m_keyboard->m_keyMap[k].m_network->SetPlayerInfo((void*)m_eventManager->m_localPlayer->m_playerInfo);
					m_globalObjects->m_keyboard->m_keyMap[k].m_network->SetServerInfo((void*)m_eventManager->m_localPlayer);
				}

				m_eventManager->Send(m_globalObjects->m_keyboard->m_keyMap[k].m_network);
			}
		}
	}

	if (!isActive)
	{
		if (((m_eventManager->m_client != nullptr) && (m_eventManager->m_client->m_connectionState == CClient::ClientState::E_CONNECTED)) ||
			((m_eventManager->m_server != nullptr) && (m_eventManager->m_server->m_localServerRunning) &&
				(m_eventManager->m_localPlayer != nullptr) && (m_eventManager->m_localPlayer->m_isRunning)))
		{
			CNetwork* n = new CNetwork(CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_IDLE,
				nullptr, 0,
				(void*)m_eventManager->m_localPlayer->m_playerInfo,
				(void*)m_eventManager->m_localPlayer);

			m_eventManager->Send(n);

			delete n;
		}
	}
}

void HandleKeyboard(WPARAM wParam)
{
	// handling keys that would be blocked by the m_console
	if (m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_network != nullptr)
	{
		switch (m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_network->m_type)
		{
		case CNetwork::E_CE_EXIT_GAME:
		{
			if ((m_eventManager->m_localPlayer != nullptr) && (m_eventManager->m_localPlayer->m_isRunning))
			{
				m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_network->SetPlayerInfo((void*)m_eventManager->m_localPlayer->m_playerInfo);
				m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_network->SetServerInfo((void*)m_eventManager->m_localPlayer);
			}

			m_eventManager->Send(m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_network);

			break;
		}
		case CNetwork::E_CE_TOGGLE_CONSOLE:
		{
			if ((m_eventManager->m_localPlayer != nullptr) && (m_eventManager->m_localPlayer->m_isRunning))
			{
				m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_network->SetPlayerInfo((void*)m_eventManager->m_localPlayer->m_playerInfo);
				m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_network->SetServerInfo((void*)m_eventManager->m_localPlayer);
			}

			m_eventManager->Send(m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_network);

			break;
		}
		default:
		{
			break;
		}
		}
	}
}

void Initialize()
{
	m_frametime = new CFrametime();


	m_console = new CConsole(m_globalObjects, "image\\button.tga", "Tahoma", 12.0f, 800.0f, 400.0f);

	m_eventManager = new CEventManager(m_globalObjects, m_console);


	m_commandListBundle[0] = new CCommandListBundle(m_globalObjects->m_errorLog, m_globalObjects->m_videoDevice);

	m_commandListBundle[0]->Add(m_globalObjects->m_videoDevice->m_commandList);

	m_commandListBundle[0]->Close();


	m_consolas12 = m_globalObjects->m_fontD12Mgr->Create("Consolas", 12.0f, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL);

	m_arial18 = m_globalObjects->m_fontD12Mgr->Create("Arial", 16.0f, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL);

	m_textImage = m_globalObjects->m_textureMgr->Create("image\\text.tga");

	m_userId = new CTextField(m_globalObjects, m_arial18, m_textImage,
		CVertex2(128.0f, 20.0f), CVertex2(400.0f, 20.0f), CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_IDLE, false);

	m_password = new CTextField(m_globalObjects, m_arial18, m_textImage,
		CVertex2(128.0f, 20.0f), CVertex2(400.0f, 60.0f), CNetwork::ClientEvent::E_CE_TO_SERVER, CNetwork::ServerEvent::E_SE_PLAYER_IDLE, true);

	m_commandListBundle[1] = new CCommandListBundle(m_globalObjects->m_errorLog, m_globalObjects->m_videoDevice);

	m_commandListBundle[1]->Add(m_userId->m_image->m_commandList);
	m_commandListBundle[1]->Add(m_password->m_image->m_commandList);

	m_commandListBundle[1]->Close();

	// Buttons
	m_buttonImage = m_globalObjects->m_textureMgr->Create("image\\button.tga");

	m_buttonManager = new CButtonManager(m_globalObjects);

	m_buttonManager->Create((HMENU)CButton::E_BTN_CONSOLE, "BTN_CONSOLE", "Console", m_buttonImage, "Calibri", 12,
		CVertex2(64, 32), CVertex2(4, 400), CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_TOGGLE_CONSOLE);

	m_buttonManager->Create((HMENU)CButton::E_BTN_WINDOWMODE, "BTN_WINDOWMODE", "FullScreen", m_buttonImage, "Calibri", 12,
		CVertex2(64, 32), CVertex2(4, 436), CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_TOGGLE_WINDOW);

	m_buttonManager->Create((HMENU)CButton::E_BTN_BROWSER, "BTN_BROWSER", "Browser", m_buttonImage, "Calibri", 12,
		CVertex2(64, 32), CVertex2(4, 472), CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_OPEN_BROWSER);

	m_buttonManager->Create((HMENU)CButton::E_BTN_EXIT, "BTN_EXIT", "X", m_buttonImage, "Calibri", 12,
		CVertex2(16, 16), CVertex2(1220, 4), CNetwork::ClientEvent::E_CE_TO_LOCAL, CNetwork::ClientEvent::E_CE_EXIT_GAME);

	// The order of these must match the Controls enum in CButton
	m_buttons[0] = m_buttonManager->Get("BTN_BROWSER");
	m_buttons[1] = m_buttonManager->Get("BTN_CONSOLE");
	m_buttons[2] = m_buttonManager->Get("BTN_EXIT");
	m_buttons[3] = m_buttonManager->Get("BTN_WINDOWMODE");

	m_commandListBundle[2] = new CCommandListBundle(m_globalObjects->m_errorLog, m_globalObjects->m_videoDevice);

	for (int i = 0; i < CButton::E_MAX_BUTTONS; i++)
	{
		m_commandListBundle[2]->Add(m_buttons[i]->m_image->m_commandList);
	}

	m_commandListBundle[2]->Close();


	// Console
	m_commandListBundle[3] = new CCommandListBundle(m_globalObjects->m_errorLog, m_globalObjects->m_videoDevice);

	m_commandListBundle[3]->Add(m_console->m_background->m_commandList);
	m_commandListBundle[3]->Close();


	// Wait for the texture uploads to complete
	m_globalObjects->m_videoDevice->WaitForGPU();

	m_globalObjects->m_soundMgr->Create("audio\\rifle.wav", 0);
	m_globalObjects->m_soundMgr->Create("audio\\button2.wav", 0);
	m_globalObjects->m_soundMgr->Create("audio\\chimes.wav", 0);
}

void LocalServerFrametime()
{
	if (m_eventManager->m_server != nullptr)
	{
		if (m_eventManager->m_server->m_localServerRunning)
		{
			m_eventManager->m_server->m_frametime->Frame();

			CLinkListNode<CServerInfo>* lln = m_eventManager->m_server->m_serverInfo->m_list;

			while (lln->m_object)
			{
				lln->m_object->m_timerReload->Frame();

				lln = lln->m_next;
			}
		}
		else
		{
			m_frametime->Frame();
		}
	}
}

void RenderFrame()
{
	m_globalObjects->m_videoDevice->ResetAndClearTargets();

	// Record 3D
	if ((m_eventManager->m_localPlayer != nullptr) && (m_eventManager->m_localPlayer->m_isRunning) && (m_eventManager->m_localPlayer->m_playerInfo->m_isRecordable))
	{
		m_currentCamera = m_eventManager->m_localPlayer->m_playerInfo->m_camera;
		
		m_currentCamera->SetPosition(&m_eventManager->m_localPlayer->m_playerInfo->m_position);

		m_currentCamera->UpdateView();

		m_eventManager->m_localPlayer->m_playerInfo->SetDirection(&m_currentCamera->m_look);


		m_eventManager->m_overhead->ClearTargets();

		m_eventManager->m_overhead->SetCameraPosition(m_currentCamera->m_position);
		m_eventManager->m_overhead->m_camera->UpdateView();

		// These are the items for the scene
		// If you don't record the command list then don't add it or if you do record it be sure to add it
		// The order of Adds determine the drawing order for the scene objects
		// If you need to draw items from back to front for alpha channel textures then add them in back to front order
		m_eventManager->m_commandListBundle->Clear();


		m_eventManager->m_terrain->SetCurrentCamera(m_currentCamera);

		m_eventManager->m_terrain->Record();

		m_eventManager->m_commandListBundle->Add(m_eventManager->m_terrain->m_commandList);



		CLinkListNode<CServerInfo>* serverInfo = m_eventManager->m_serverInfo->m_list;

		// Wait if the list is being updated by the event manager thread
		while (m_eventManager->m_serverInfo->m_isBeingUpdated)
		{
		}

		// Put the list in a wait state so the event manager thread can't update it
		m_eventManager->m_serverInfo->m_isBeingTraversed = true;

		while (serverInfo->m_object)
		{
			// Might as well handle the bots here when it is running single player
			if ((serverInfo->m_object->m_isBot) && (m_eventManager->m_server->m_localServerRunning))
			{
				m_eventManager->m_server->BotInference(serverInfo->m_object, m_eventManager->m_server->m_frametime);
			}

			if (serverInfo->m_object->m_playerInfo->m_isRecordable)
			{
				serverInfo->m_object->m_playerInfo->m_model->SetPosition(&serverInfo->m_object->m_playerInfo->m_position);

				// no pitch no roll just ya'll
				serverInfo->m_object->m_playerInfo->m_model->SetRotation(
					0.0f,
					serverInfo->m_object->m_playerInfo->m_direction.PointToDegree().p.y,
					0.0f);

				serverInfo->m_object->m_playerInfo->m_model->SetCurrentCamera(m_currentCamera);
				serverInfo->m_object->m_playerInfo->m_model->SetOverheadCamera(m_eventManager->m_overhead->m_camera);
				serverInfo->m_object->m_playerInfo->m_model->Update();
				serverInfo->m_object->m_playerInfo->m_model->Record();

				m_eventManager->m_commandListBundle->Add(serverInfo->m_object->m_playerInfo->m_model->m_commandList);
			}

			serverInfo = serverInfo->m_next;
		}

		// Free the list up to the event manager thread for updates
		m_eventManager->m_serverInfo->m_isBeingTraversed = false;


		// Must delete each cube object's list
		for (UINT pz = 0; pz < m_eventManager->m_visibility->m_gridHeight; pz++)
		{
			for (UINT px = 0; px < m_eventManager->m_visibility->m_gridWidth; px++)
			{
				CLinkList<CObject>* collectables = (CLinkList<CObject>*)m_eventManager->m_visibility->m_collectables->GetElement(2, px, pz);

				if (collectables->m_list != nullptr)
				{
					CLinkListNode<CObject>* collectable = (CLinkListNode<CObject>*)collectables->m_list;

					while (collectable->m_object)
					{
						if (!collectable->m_object->m_limboTimer->m_isReloading)
						{
							collectable->m_object->SetCurrentCamera(m_currentCamera);
							collectable->m_object->SetOverheadCamera(m_eventManager->m_overhead->m_camera);
							collectable->m_object->Animation(m_eventManager->m_server->m_frametime);
							collectable->m_object->Update();
							collectable->m_object->Record();

							m_eventManager->m_commandListBundle->Add(collectable->m_object->m_commandList);
						}
						else
						{
							collectable->m_object->m_limboTimer->Frame();
						}

						collectable = collectable->m_next;
					}
				}
			}
		}


		m_eventManager->m_overhead->Record();

		m_eventManager->m_commandListBundle->Add(m_eventManager->m_overhead->m_commandList);
	}

	// Record HUD
	for (int i = 0; i < CButton::E_MAX_BUTTONS; i++)
	{
		m_buttons[i]->Record();
	}

	m_userId->Record();

	m_password->Record();

	if (m_console->m_isVisible)
	{
		m_console->DisplayConsole();
	}


	// Perform the command lists
	// Video Device is always first
	m_commandListBundle[0]->Close();
	m_commandListBundle[0]->Perform();

	// 3D
	// Event Manager
	if ((m_eventManager->m_localPlayer != nullptr) && (m_eventManager->m_localPlayer->m_playerInfo->m_isRecordable))
	{
		m_eventManager->m_commandListBundle->Close();
		m_eventManager->m_commandListBundle->Perform();
	}

	// HUD
	// Text Fields
	m_commandListBundle[1]->Close();
	m_commandListBundle[1]->Perform();

	// Buttons
	m_commandListBundle[2]->Close();
	m_commandListBundle[2]->Perform();

	if (m_console->m_isVisible)
	{
		m_commandListBundle[3]->Close();
		m_commandListBundle[3]->Perform();
	}

	RenderText();

	m_globalObjects->m_videoDevice->MoveToNextFrame();
}

void RenderText()
{
	// get ready to render text
	m_globalObjects->m_videoDevice->Init2DDraw();

	wchar_t text[50] = {};

	CVertex2 position(10.0f, 10.0f);
	CVertex2 size(256.0f, 32.0f);

	if ((m_eventManager->m_server != nullptr) && (m_eventManager->m_server->m_localServerRunning))
	{
		swprintf_s(text, 50, L"FPS: %i %f",
			m_eventManager->m_server->m_frametime->m_rframecount,
			m_eventManager->m_server->m_frametime->m_frametime);

		m_consolas12->Draw(text, position, size, m_globalObjects->m_color->SunYellow);

		position.p.y += m_consolas12->m_height;
	}
	else
	{
		swprintf_s(text, 50, L"FPS: %i",
			m_frametime->m_rframecount);

		m_consolas12->Draw(text, position, size, m_globalObjects->m_color->SunYellow);

		position.p.y += m_consolas12->m_height;
	}


	memset((void*)text, 0x00, 50);

	swprintf_s(text, 50, L"MButton: %i %i %i",
		m_globalObjects->m_mouse->m_rgbButtons[0],
		m_globalObjects->m_mouse->m_rgbButtons[1],
		m_globalObjects->m_mouse->m_rgbButtons[2]);

	m_consolas12->Draw(text, position, size, m_globalObjects->m_color->SunYellow);


	position.p.y += m_consolas12->m_height;

	memset((void*)text, 0x00, 50);

	swprintf_s(text, 50, L"MP1: %04.0f %04.0f",
		m_globalObjects->m_mouse->m_position.p.x,
		m_globalObjects->m_mouse->m_position.p.y);

	m_consolas12->Draw(text, position, size, m_globalObjects->m_color->SunYellow);


	position.p.y += m_consolas12->m_height;

	memset((void*)text, 0x00, 50);

	swprintf_s(text, 50, L"MP2: %ld %ld", m_globalObjects->m_mouse->m_physicalPosition.x, m_globalObjects->m_mouse->m_physicalPosition.y);

	m_consolas12->Draw(text, position, size, m_globalObjects->m_color->SunYellow);


	if ((m_eventManager->m_localPlayer != nullptr) && (m_eventManager->m_localPlayer->m_playerInfo->m_isRecordable))
	{
		if (m_currentCamera != nullptr)
		{
			CVertex yp(m_currentCamera->m_look.x, m_currentCamera->m_look.y, m_currentCamera->m_look.z);
			CVertex r(m_currentCamera->m_right.x, m_currentCamera->m_right.y, m_currentCamera->m_right.z);

			position.p.y += m_consolas12->m_height;

			memset((void*)text, 0x00, 50);

			swprintf_s(text, 50, L"Y:%5.2f P:%5.2f R:%5.2f", yp.PointToDegree().p.y, yp.PointToDegree().p.x, r.PointToDegree().p.z);

			m_consolas12->Draw(text, position, size, m_globalObjects->m_color->SunYellow);


			position.p.y += m_consolas12->m_height;

			memset((void*)text, 0x00, 50);

			swprintf_s(text, 50, L"Position: %06.2f %06.2f %06.2f", m_currentCamera->m_position.x, m_currentCamera->m_position.y, m_currentCamera->m_position.z);

			m_consolas12->Draw(text, position, size, m_globalObjects->m_color->SunYellow);


			if ((m_eventManager->m_server) && (m_eventManager->m_server->m_serverEnvironment))
			{
				int px = (int)(m_currentCamera->m_position.x +
					(m_eventManager->m_server->m_serverEnvironment->m_width / 2.0f)) / m_eventManager->m_server->m_serverEnvironment->m_gridUnits;
				int pz = (int)(m_currentCamera->m_position.z +
					(m_eventManager->m_server->m_serverEnvironment->m_height / 2.0f)) / m_eventManager->m_server->m_serverEnvironment->m_gridUnits;

				position.p.y += m_consolas12->m_height;

				memset((void*)text, 0x00, 50);

				swprintf_s(text, 50, L"Sector: %i %i %i %i",
					m_eventManager->m_server->m_serverEnvironment->m_gridWidth,
					m_eventManager->m_server->m_serverEnvironment->m_gridHeight,
					px,
					pz);

				m_consolas12->Draw(text, position, size, m_globalObjects->m_color->SunYellow);
			}

			if (m_eventManager->m_localPlayer != nullptr)
			{
				position.p.y += m_consolas12->m_height;

				memset((void*)text, 0x00, 50);

				swprintf_s(text, 50, L"Velocity:%06.2f Idle:%06.2f",
					m_eventManager->m_localPlayer->m_velocity,
					m_eventManager->m_localPlayer->m_idleTime);

				m_consolas12->Draw(text, position, size, m_globalObjects->m_color->SunYellow);
			}
		}
	}


	for (int i = 0; i < CButton::E_MAX_BUTTONS; i++)
	{
		m_buttons[i]->DisplayText();
	}

	m_userId->DisplayText();

	m_password->DisplayText();

	if (m_console->m_isVisible)
	{
		m_console->DisplayText();
	}

	// end drawing text
	m_globalObjects->m_videoDevice->End2DDraw();
}
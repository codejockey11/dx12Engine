#include "standard.h"

#include "CButtonManager.h"
#include "CCamera.h"
#include "CCommandListBundle.h"
#include "CConsole.h"
#include "CGlobalObjects.h"
#include "CEventManager.h"
#include "CTextField.h"
#include "CWindow.h"

CGlobalObjects* m_globalObjects;
CEventManager* m_eventManager;
CConsole* m_console;
CWindow* m_browser;
CCamera* m_currentCamera;

CTexture* m_buttonImage;
CTexture* m_textImage;

CD12Font* m_consolas12;
CD12Font* m_arial18;

CButton* m_buttons[CButton::MAX_BUTTONS];

CButtonManager* m_buttonManager;

constexpr auto MAX_BUNDLES = 5;

CCommandListBundle* m_commandListBundle[MAX_BUNDLES];

CTextField* m_userId;
CTextField* m_password;

void Initialize();
void Cleanup();
void GetControllerActivity();
void RenderFrame();
void RenderText();
void HandleKeyboard(WPARAM wParam);
bool CheckButtonMouseOver();
bool CheckTextFieldMouseOver();
void CheckWM_KEYDOWN(CTextField* textField, WPARAM w);
void CheckWM_CHAR(CTextField* textField, WPARAM w);

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BrowserWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	HRESULT hr = OleInitialize(0);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	m_globalObjects = new CGlobalObjects(hInstance, "dx12EngineClass", "dx12Engine", MainWindowProc, CWindow::WT_MAIN, "log.txt", "config.txt");

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
			if (m_browser == nullptr)
			{
				m_globalObjects->m_frametime->Frame();

				CEvent evt(-1, -1, CEvent::ET_LOCAL, CEvent::ET_DRAW);

				m_eventManager->Send(&evt);

				GetControllerActivity();
			}
		}
	}

	Cleanup();


	OleUninitialize();

	return (int)msg.wParam;
}

void Initialize()
{
	m_console = new CConsole(m_globalObjects, "image\\console.tga", "Tahoma", 12.0f, 800.0f, 400.0f);

	m_eventManager = new CEventManager(m_globalObjects, m_console);


	m_commandListBundle[0] = new CCommandListBundle(m_globalObjects->m_errorLog, m_globalObjects->m_videoDevice);

	m_commandListBundle[0]->Add(m_globalObjects->m_videoDevice->m_commandList);

	m_commandListBundle[0]->Close();


	m_consolas12 = m_globalObjects->m_fontD12Mgr->Create("Consolas", 12.0f, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL);

	m_arial18 = m_globalObjects->m_fontD12Mgr->Create("Arial", 18.0f, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL);

	m_textImage = m_globalObjects->m_textureMgr->Create("image\\text.tga");

	m_userId = new CTextField(m_globalObjects, m_arial18, m_textImage,
		CVertex2(128.0f, 20.0f), CVertex2(400.0f, 20.0f), CEvent::ET_LOCAL, CEvent::ET_ATTACK, false);

	m_password = new CTextField(m_globalObjects, m_arial18, m_textImage,
		CVertex2(128.0f, 20.0f), CVertex2(400.0f, 60.0f), CEvent::ET_LOCAL, CEvent::ET_ATTACK, true);

	m_commandListBundle[1] = new CCommandListBundle(m_globalObjects->m_errorLog, m_globalObjects->m_videoDevice);

	m_commandListBundle[1]->Add(m_userId->m_image->m_commandList);
	m_commandListBundle[1]->Add(m_password->m_image->m_commandList);

	m_commandListBundle[1]->Close();

	// Buttons
	m_buttonImage = m_globalObjects->m_textureMgr->Create("image\\button.tga");

	m_buttonManager = new CButtonManager(m_globalObjects);

	m_buttonManager->Create((HMENU)CButton::BTN_CONSOLE, "BTN_CONSOLE", "Console", m_buttonImage, m_consolas12,
		CVertex2(64, 32), CVertex2(4, 400), CEvent::ET_LOCAL, CEvent::ET_TOGGLECONSOLE);

	m_buttonManager->Create((HMENU)CButton::BTN_WINDOWMODE, "BTN_WINDOWMODE", "Fullscreen", m_buttonImage, m_consolas12,
		CVertex2(64, 32), CVertex2(4, 436), CEvent::ET_LOCAL, CEvent::ET_WINDOWMODE);

	m_buttonManager->Create((HMENU)CButton::BTN_BROWSER, "BTN_BROWSER", "Browser", m_buttonImage, m_consolas12,
		CVertex2(64, 32), CVertex2(4, 472), CEvent::ET_LOCAL, CEvent::ET_EMPTY);

	m_buttonManager->Create((HMENU)CButton::BTN_EXIT, "BTN_EXIT", "X", m_buttonImage, m_consolas12,
		CVertex2(16, 16), CVertex2(1220, 4), CEvent::ET_LOCAL, CEvent::ET_EXIT);

	m_buttons[0] = m_buttonManager->Get("BTN_CONSOLE");
	m_buttons[1] = m_buttonManager->Get("BTN_EXIT");
	m_buttons[2] = m_buttonManager->Get("BTN_WINDOWMODE");
	m_buttons[3] = m_buttonManager->Get("BTN_BROWSER");

	m_commandListBundle[2] = new CCommandListBundle(m_globalObjects->m_errorLog, m_globalObjects->m_videoDevice);

	for (int i = 0; i < CButton::MAX_BUTTONS; i++)
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

	m_globalObjects->m_soundMgr->MakeSound("audio\\rifle.wav", 0);
	m_globalObjects->m_soundMgr->MakeSound("audio\\button2.wav", 0);
	m_globalObjects->m_soundMgr->MakeSound("audio\\chimes.wav", 0);
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

	for (int i = 0; i < MAX_BUNDLES; i++)
	{
		delete m_commandListBundle[i];
	}
	
	delete m_eventManager;
	delete m_console;
	delete m_globalObjects;
}

void GetControllerActivity()
{
	bool noActivity = true;

	m_globalObjects->m_mouse->GetState();

	for (int k = 0; k < 4; k++)
	{
		if ((m_globalObjects->m_mouse->m_keyMap[k].m_count > 0) && (m_globalObjects->m_mouse->m_keyMap[k].m_repeater == CMouseDevice::KR_REPEATER))
		{
			if (m_globalObjects->m_mouse->m_keyMap[k].m_event->m_type == CEvent::ET_FPSMOVE)
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
		}
		else if ((m_globalObjects->m_mouse->m_keyMap[k].m_count == 1) && (m_globalObjects->m_mouse->m_keyMap[k].m_repeater == CMouseDevice::KR_ONESHOT))
		{
			bool wasClicked = false;

			for (int i = 0; i < CButton::MAX_BUTTONS; i++)
			{
				if (m_buttons[i]->m_isMouseOver)
				{
					// m_buttons are handled in the message callback
					wasClicked = true;

					continue;
				}
			}

			if (m_userId->m_isMouseOver)
			{
			}
			else if (m_password->m_isMouseOver)
			{
			}
			else if (!wasClicked)
			{
				noActivity = false;

				memcpy((void*)m_globalObjects->m_mouse->m_keyMap[k].m_event->m_playerInfo, (void*)m_eventManager->m_redTeamInfo[0], sizeof(CPlayerInfo));

				m_eventManager->Send(m_globalObjects->m_mouse->m_keyMap[k].m_event);
			}
		}
	}


	m_globalObjects->m_keyboard->GetState();

	for (int k = 0; k < 256; k++)
	{
		if ((m_globalObjects->m_keyboard->m_keyMap[k].m_count > 0) && (m_globalObjects->m_keyboard->m_keyMap[k].m_repeater == CKeyboardDevice::KR_REPEATER))
		{
			noActivity = false;

			memcpy((void*)m_globalObjects->m_keyboard->m_keyMap[k].m_event->m_playerInfo, (void*)m_eventManager->m_redTeamInfo[0], sizeof(CPlayerInfo));
			
			m_eventManager->Send(m_globalObjects->m_keyboard->m_keyMap[k].m_event);
		}
		else if ((m_globalObjects->m_keyboard->m_keyMap[k].m_count == 1) && (m_globalObjects->m_keyboard->m_keyMap[k].m_repeater == CKeyboardDevice::KR_ONESHOT))
		{
			if (m_globalObjects->m_keyboard->m_keyMap[k].m_event)
			{
				noActivity = false;

				m_eventManager->Send(m_globalObjects->m_keyboard->m_keyMap[k].m_event);
			}
		}
	}

	if (m_eventManager->m_client != nullptr)
	{
		if ((noActivity) && (m_eventManager->m_client->m_isRunning))
		{
			CEvent* clientEvent = new CEvent(-1, -1, CEvent::ET_SERVER, CEvent::CServerEvent::SE_IDLEPLAYER);

			m_eventManager->Send(clientEvent);

			delete clientEvent;
		}
	}
}

void RenderFrame()
{
	m_globalObjects->m_videoDevice->ResetAndClearTargets();

	// Draw 3D
	if ((m_eventManager->m_redTeam[0] != nullptr) && (m_eventManager->m_redTeamInfo[0]->m_isReady))
	{
		m_eventManager->m_overhead->ClearTargets();

		m_currentCamera = m_eventManager->m_redTeam[0]->m_camera;

		m_currentCamera->UpdateView();

		m_eventManager->m_redTeamInfo[0]->m_direction.p.x = m_eventManager->m_redTeam[0]->m_camera->m_look.x;
		m_eventManager->m_redTeamInfo[0]->m_direction.p.y = m_eventManager->m_redTeam[0]->m_camera->m_look.y;
		m_eventManager->m_redTeamInfo[0]->m_direction.p.z = m_eventManager->m_redTeam[0]->m_camera->m_look.z;


		m_eventManager->m_overhead->SetCameraPosition(m_currentCamera->m_position);
		m_eventManager->m_overhead->m_camera->UpdateView();

		
		m_eventManager->m_wavefront->SetCurrentCamera(m_currentCamera);
		m_eventManager->m_wavefront->SetOverheadCamera(m_eventManager->m_overhead->m_camera);

		m_eventManager->m_dynamicModel->SetCurrentCamera(m_currentCamera);
		m_eventManager->m_dynamicModel->SetOverheadCamera(m_eventManager->m_overhead->m_camera);

		m_eventManager->m_staticModel->SetCurrentCamera(m_currentCamera);
		m_eventManager->m_staticModel->SetOverheadCamera(m_eventManager->m_overhead->m_camera);
		
		m_eventManager->m_terrain->SetCurrentCamera(m_currentCamera);


		m_eventManager->m_terrain->Record();

		m_eventManager->m_wavefront->Record();

		m_eventManager->m_dynamicModel->Record();

		m_eventManager->m_staticModel->Record();

		m_eventManager->m_overhead->Record();


		// These are the items for the scene
		// If you don't record the command list then don't add it or if you do record it be sure to add it
		// The order of Adds determine the drawing order for the scene objects
		// If you need to draw items from back to front then add them in back to front order
		m_eventManager->m_commandListBundle->Clear();

		m_eventManager->m_commandListBundle->Add(m_eventManager->m_terrain->m_commandList);
		m_eventManager->m_commandListBundle->Add(m_eventManager->m_dynamicModel->m_commandList);
		m_eventManager->m_commandListBundle->Add(m_eventManager->m_staticModel->m_commandList);
		m_eventManager->m_commandListBundle->Add(m_eventManager->m_wavefront->m_commandList);
		m_eventManager->m_commandListBundle->Add(m_eventManager->m_overhead->m_commandList);
	}

	// Draw HUD
	for (int i = 0; i < CButton::MAX_BUTTONS; i++)
	{
		m_buttons[i]->Record();
	}
	
	m_userId->Record();

	m_password->Record();

	if (m_console->m_isVisible)
	{
		m_console->DisplayConsole();
	}


	// Execute the command lists
	// Video Device is always first
	m_commandListBundle[0]->Close();
	m_commandListBundle[0]->Execute();

	// 3D
	// Event Manager
	if (m_eventManager->m_redTeam[0] != nullptr)
	{
		m_eventManager->m_commandListBundle->Close();
		m_eventManager->m_commandListBundle->Execute();
	}

	// HUD
	// Text Fields
	m_commandListBundle[1]->Close();
	m_commandListBundle[1]->Execute();

	// Buttons
	m_commandListBundle[2]->Close();
	m_commandListBundle[2]->Execute();

	if (m_console->m_isVisible)
	{
		m_commandListBundle[3]->Close();
		m_commandListBundle[3]->Execute();
	}

	RenderText();

	m_globalObjects->m_videoDevice->MoveToNextFrame();
}

void RenderText()
{
	// get ready to render text
	m_globalObjects->m_videoDevice->Init2DDraw();


	char text[50] = {};

	CVertex2 position(10.0f, 10.0f);
	CVertex2 size(256.0f, 32.0f);

	sprintf_s(text, 50, "FPS: %i %f", m_globalObjects->m_frametime->m_rframecount, m_globalObjects->m_frametime->m_rframetime);

	m_consolas12->Draw(text, position, size, &SunYellow);

	position.p.y += m_consolas12->m_height;

	memset((void*)text, 0x00, 50);

	sprintf_s(text, 50, "MButton: %i %i %i", m_globalObjects->m_mouse->m_rgbButtons[0], m_globalObjects->m_mouse->m_rgbButtons[1], m_globalObjects->m_mouse->m_rgbButtons[2]);

	m_consolas12->Draw(text, position, size, &SunYellow);


	position.p.y += m_consolas12->m_height;

	memset((void*)text, 0x00, 50);

	sprintf_s(text, 50, "MP1: %04.0f %04.0f", m_globalObjects->m_mouse->m_position.p.x, m_globalObjects->m_mouse->m_position.p.y);

	m_consolas12->Draw(text, position, size, &SunYellow);


	position.p.y += m_consolas12->m_height;

	memset((void*)text, 0x00, 50);

	sprintf_s(text, 50, "MP2: %ld %ld", m_globalObjects->m_mouse->m_physicalPosition.x, m_globalObjects->m_mouse->m_physicalPosition.y);

	m_consolas12->Draw(text, position, size, &SunYellow);


	if ((m_eventManager->m_redTeam[0] != nullptr) && (m_eventManager->m_redTeamInfo[0]->m_isReady))
	{
		if (m_currentCamera != nullptr)
		{
			CVertex yp(m_currentCamera->m_look.x, m_currentCamera->m_look.y, m_currentCamera->m_look.z);
			CVertex r(m_currentCamera->m_right.x, m_currentCamera->m_right.y, m_currentCamera->m_right.z);

			position.p.y += m_consolas12->m_height;

			memset((void*)text, 0x00, 50);

			sprintf_s(text, 50, "Y:%5.2f P:%5.2f R:%5.2f", yp.PointToDegree().p.y, yp.PointToDegree().p.x, r.PointToDegree().p.z);

			m_consolas12->Draw(text, position, size, &SunYellow);


			position.p.y += m_consolas12->m_height;

			memset((void*)text, 0x00, 50);

			sprintf_s(text, 50, "Position: %06.2f %06.2f %06.2f", m_currentCamera->m_position.x, m_currentCamera->m_position.y, m_currentCamera->m_position.z);

			m_consolas12->Draw(text, position, size, &SunYellow);


			int px = (int)((m_currentCamera->m_position.x +
				(((m_eventManager->m_terrain->m_width - 1) / 2.0f) *
					(m_eventManager->m_terrain->m_primSize))) / (m_eventManager->m_terrain->m_primSize));

			int pz = (int)((m_currentCamera->m_position.z +
				(((m_eventManager->m_terrain->m_height - 1) / 2.0f) *
					(m_eventManager->m_terrain->m_primSize))) / (m_eventManager->m_terrain->m_primSize));

			position.p.y += m_consolas12->m_height;

			memset((void*)text, 0x00, 50);

			sprintf_s(text, 50, "Sector: %i %i", px, pz);

			m_consolas12->Draw(text, position, size, &SunYellow);
		}
	}


	for (int i = 0; i < CButton::MAX_BUTTONS; i++)
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

void HandleKeyboard(WPARAM wParam)
{
	// handling keys that would be blocked by the m_console
	if (m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_event != nullptr)
	{
		switch (m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_event->m_type)
		{
			case CEvent::ET_EXIT:
			{
				m_eventManager->Send(m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_event);

				break;
			}
			case CEvent::ET_TOGGLECONSOLE:
			{
				m_eventManager->Send(m_globalObjects->m_keyboard->m_keyMap[(int)wParam].m_event);

				break;
			}
			default:
			{
				break;
			}
		}
	}
}

bool CheckButtonMouseOver()
{
	for (int i = 0; i < CButton::MAX_BUTTONS; i++)
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

void CheckWM_CHAR(CTextField* textField, WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		CEvent* evt = textField->GetEvent();

		if (evt != nullptr)
		{
			m_eventManager->Send(evt);
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

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	// Regular first person shooter mouse move
	/*
	case WM_MOUSEMOVE:
	{
		if (m_currentCamera)
		{
			m_globalObjects->m_mouse->MouseMove();

			m_currentCamera->UpdateRotation((m_globalObjects->m_mouse->m_lY * m_globalObjects->m_mouse->m_sensitivity),
			(m_globalObjects->m_mouse->m_lX * m_globalObjects->m_mouse->m_sensitivity),
			(m_globalObjects->m_mouse->m_lZ * m_globalObjects->m_mouse->m_sensitivity));

			m_globalObjects->m_mouse->FirstPersonReset();
		}

		return 0;
	}
	*/
	case CEvent::WM_DRAW:
	{
		RenderFrame();

		return 0;
	}
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

		m_globalObjects->m_mouse->MouseClick(CMouseDevice::RMB, 1);

		return 0;

		break;
	}
	case WM_RBUTTONUP:
	{
		if (m_console->m_isVisible)
		{
			return 0;
		}

		if (m_browser != nullptr)
		{
			return 0;
		}

		m_globalObjects->m_mouse->SetLastPosition();

		m_globalObjects->m_mouse->MouseClick(CMouseDevice::RMB, 0);

		return 0;

		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (CheckButtonMouseOver())
		{
		}
		else if (CheckTextFieldMouseOver())
		{
		}
		else if (m_console->m_isVisible)
		{
			return 0;
		}

		if (m_browser != nullptr)
		{
			return 0;
		}


		m_globalObjects->m_mouse->MouseClick(CMouseDevice::LMB, 1);

		return 0;

		break;
	}
	case WM_LBUTTONUP:
	{
		if (CheckButtonMouseOver())
		{
		}
		else if (m_console->m_isVisible)
		{
			return 0;
		}

		m_globalObjects->m_mouse->MouseClick(CMouseDevice::LMB, 0);

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

		m_globalObjects->m_mouse->MouseClick(CMouseDevice::MMB, 1);

		return 0;

		break;
	}
	case WM_MBUTTONUP:
	{
		if (m_console->m_isVisible)
		{
			return 0;
		}

		m_globalObjects->m_mouse->MouseClick(CMouseDevice::MMB, 0);

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
				m_console->Paging(CConsole::PAGE_BACKWARD);

				return 0;
			}
			if (wheelDelta < 0)
			{
				m_console->Paging(CConsole::PAGE_FORWARD);

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
				CEvent* clientEvent = m_console->CommandFromConsole(m_eventManager->m_redTeamInfo[0]);

				if (clientEvent != nullptr)
				{
					m_eventManager->Send(clientEvent);
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
				m_console->Paging(CConsole::PAGE_BACKWARD);

				return 0;

				break;
			}
			case VK_NEXT:
			{
				m_console->Paging(CConsole::PAGE_FORWARD);

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
			// keydown is not ASCII
			default:
			{
				HandleKeyboard(wParam);

				// everything else happens with WM_CHAR

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
		case CButton::BTN_WINDOWMODE:
		{
			if (m_globalObjects->m_window->m_windowed)
			{
				for (int i = 0; i < CButton::MAX_BUTTONS; i++)
				{
					m_buttons[i]->m_windowsButton->FullscreenWindowsButton();
				}
			}
			else
			{
				for (int i = 0; i < CButton::MAX_BUTTONS; i++)
				{
					m_buttons[i]->m_windowsButton->WindowWindowsButton();
				}
			}

			break;
		}
		case CButton::BTN_BROWSER:
		{
			if (m_browser)
			{
				delete m_browser;
			}

			m_browser = new CWindow(m_globalObjects->m_window->m_hWnd,
				m_globalObjects->m_window->m_hInstance,
				true,
				CWindow::WT_BROWSER,
				m_globalObjects->m_window->m_width, m_globalObjects->m_window->m_height,
				"browserClass", "Browser",
				BrowserWindowProc);

			break;
		}
		}

		m_buttons[(int)wParam - CButton::ControlsCount]->m_soundClicked->StartSound();

		m_eventManager->Send(m_buttons[(int)wParam - CButton::ControlsCount]->m_event);

		// Focus must be set to parent after button click
		SetFocus(m_globalObjects->m_window->m_hWnd);

		return 0;

		break;
	}
	case WM_CLOSE:
	{
		if (m_globalObjects->m_window->m_hWnd)
		{
			DestroyWindow(m_globalObjects->m_window->m_hWnd);
		}

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

LRESULT CALLBACK BrowserWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case CWindow::BTN_BACK:
		{
			m_browser->m_browser->GoBack();

			break;
		}
		case CWindow::BTN_FORWARD:
		{
			m_browser->m_browser->GoForward();

			break;
		}
		case CWindow::BTN_REFRESH:
		{
			m_browser->m_browser->Refresh();

			break;
		}
		case CWindow::BTN_GO:
		{
			TCHAR* szUrl = new TCHAR[1024];

			GetWindowText(m_browser->m_hWndAddressBar, szUrl, 1024);

			m_browser->m_browser->Navigate(szUrl);

			break;
		}
		case CWindow::BTN_CLOSE:
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
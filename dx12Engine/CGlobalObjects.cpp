#include "CGlobalObjects.h"

CGlobalObjects::CGlobalObjects()
{
	memset(this, 0x00, sizeof(CGlobalObjects));
}

CGlobalObjects::CGlobalObjects(HINSTANCE hInstance, const char* className, const char* title, WNDPROC wndProc, BYTE type, const char* logFilename, const char* configFilename)
{
	memset(this, 0x00, sizeof(CGlobalObjects));

	m_className = new CString(className);
	m_title = new CString(title);

	m_config = new CConfiguration(configFilename);

	if (m_config->m_items[CConfiguration::CF_WINDOWED].m_value == 1.0f)
	{
		m_window = new CWindow(nullptr, hInstance,
			true,
			type,
			(UINT)m_config->m_items[CConfiguration::CF_SCREENWIDTH].m_value,
			(UINT)m_config->m_items[CConfiguration::CF_SCREENHEIGHT].m_value,
			m_className->GetText(),
			m_className->GetText(),
			wndProc);

	}
	else
	{
		m_window = new CWindow(nullptr, hInstance,
			false,
			type,
			(UINT)m_config->m_items[CConfiguration::CF_SCREENWIDTH].m_value,
			(UINT)m_config->m_items[CConfiguration::CF_SCREENHEIGHT].m_value,
			m_className->GetText(),
			m_className->GetText(),
			wndProc);
	}

	if (!m_window->m_hWnd)
	{
		return;
	}

	ShowWindow(m_window->m_hWnd, SW_SHOWNORMAL);

	SetCursorPos(m_window->m_middle.x, m_window->m_middle.y);


	m_errorLog = new CErrorLog(logFilename);

	m_mouse = new CMouseDevice(m_errorLog, m_window, m_config->m_items[CConfiguration::CF_MOUSESENSITIVITY].m_value);

	m_keyboard = new CKeyboardDevice();

	m_videoDevice = new CVideoDevice(m_errorLog, m_window);
	m_fontD12Mgr = new CD12FontManager(m_errorLog, m_videoDevice, m_window);
	m_soundDevice = new CSoundDevice(m_errorLog);
	m_textureMgr = new CTextureManager(m_errorLog, m_videoDevice);
	m_shaderMgr = new CShaderManager(m_errorLog);
	m_soundMgr = new CSoundManager(m_errorLog, m_soundDevice);
	m_modelManager = new CModelManager(m_errorLog);
	m_wavefrontManager = new CWavefrontManager(m_errorLog);
	m_frametime = new CFrametime();
}

CGlobalObjects::~CGlobalObjects()
{
	delete m_frametime;
	delete m_wavefrontManager;
	delete m_modelManager;
	delete m_soundMgr;
	delete m_shaderMgr;
	delete m_textureMgr;
	delete m_soundDevice;
	delete m_fontD12Mgr;
	delete m_videoDevice;
	delete m_keyboard;
	delete m_mouse;
	delete m_errorLog;

	delete m_window;
	delete m_config;
	delete m_title;
	delete m_className;
}
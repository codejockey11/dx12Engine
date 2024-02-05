#include "CButton.h"

/*
*/
CButton::CButton()
{
	memset(this, 0x00, sizeof(CButton));
}

/*
*/
CButton::CButton(CGlobalObjects* globalObjects, HMENU hMenu, const char* name, const char* text,
	CTexture* image, const char* fontName, float fontSize, CVertex2 size, CVertex2 position, BYTE audience, BYTE type)
{
	memset(this, 0x00, sizeof(CButton));

	m_globalObjects = globalObjects;

	m_size = size;
	m_position = position;

	m_name = new CString(name);

	m_text = new CString(text);

	// Windows button definition for interaction with the message callback.
	m_windowsButton = new CWindowsButton(globalObjects, hMenu, name, size, position);

	m_image = new CImage(globalObjects, image, size, position);

	m_font = m_globalObjects->m_fontD12Mgr->Create(fontName, fontSize, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL);

	m_network = new CNetwork(audience, type, nullptr, 0, nullptr, nullptr);

	m_soundOff = m_globalObjects->m_soundMgr->Create("audio\\button2.wav", false);
	m_soundOn = m_globalObjects->m_soundMgr->Create("audio\\logon.wav", false);
	m_soundClicked = m_globalObjects->m_soundMgr->Create("audio\\heli.wav", false);

	m_soundOffPlayed = true;
	m_soundOnPlayed = false;

	m_isInitialized = true;
}

/*
*/
CButton::~CButton()
{
	delete m_windowsButton;
	delete m_image;
	delete m_text;
	delete m_name;
}

/*
*/
void CButton::Active()
{
	// Shader heap constant buffers for some kind of effect
	m_image->m_floats->m_values[0] = 1.0f;

	m_isMouseOver = true;

	if (m_soundOnPlayed == false)
	{
		m_soundOffPlayed = false;

		m_soundOnPlayed = true;

		m_soundOn->StartSound();
	}

	// If you are using the windows callback then soundClicked->StartSound(); should be called during WM_COMMAND
	//if (m_globalObjects->mouse->keyMap[CMouseDevice::E_LMB]->count == 1)
	//{
		//m_soundClicked->StartSound();
	//}
}

/*
*/
void CButton::DisplayText()
{
	SIZE s = m_font->TextDimensions(m_text->GetText());

	CVertex2 position(m_image->m_position.p.x + (m_image->m_size.p.x / 2.0f) - (s.cx / 2.0f),
		m_image->m_position.p.y + (m_image->m_size.p.y / 2.0f) - (s.cy / 2.0f));

	if (m_isMouseOver)
	{
		m_font->Draw(m_text->GetWText(), position, m_image->m_size, m_globalObjects->m_color->SunYellow);
	}
	else
	{
		m_font->Draw(m_text->GetWText(), position, m_image->m_size, m_globalObjects->m_color->ZombieGreen);
	}
}

/*
*/
void CButton::Inactive()
{
	// Shader heap constant buffers for some kind of effect
	m_image->m_floats->m_values[0] = 0.75f;

	m_isMouseOver = false;

	if (m_soundOffPlayed == false)
	{
		m_soundOffPlayed = true;

		m_soundOnPlayed = false;

		//m_soundOff->StartSound();
	}
}

/*
*/
void CButton::Record()
{
	if (m_image->m_box->CheckPointInBox(&m_globalObjects->m_mouse->m_position))
	{
		CButton::Active();
	}
	else
	{
		CButton::Inactive();
	}

	m_image->Draw();
}
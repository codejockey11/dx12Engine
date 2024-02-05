#include "CTextField.h"

/*
*/
CTextField::CTextField()
{
	memset(this, 0x00, sizeof(CTextField));
}

/*
*/
CTextField::CTextField(CGlobalObjects* globalObjects, CD12Font* font,
	CTexture* texture, CVertex2 size, CVertex2 position, BYTE audience, BYTE type, bool mask)
{
	memset(this, 0x00, sizeof(CTextField));

	m_globalObjects = globalObjects;

	m_font = font;

	m_value = new CString(CTextField::E_MAX_LENGTH);

	m_image = new CImage(m_globalObjects, texture, size, position);

	m_network = new CNetwork(audience, type, nullptr, 0, nullptr, nullptr);

	m_mask = mask;

	m_soundOff = m_globalObjects->m_soundMgr->Create("audio\\button2.wav", false);

	m_soundOn = m_globalObjects->m_soundMgr->Create("audio\\logon.wav", false);

	m_soundOffPlayed = true;
	m_soundOnPlayed = false;

	m_isActive = false;
}

/*
*/
CTextField::~CTextField()
{
	delete m_network;
	delete m_image;
	delete m_value;
}

/*
*/
void CTextField::Active()
{
	// Shader heap constant buffers for some kind of effect
	m_image->m_floats->m_values[0] = 1.0f;

	m_isMouseOver = true;

	if (m_globalObjects->m_mouse->m_keyMap[CMouseDevice::E_LMB].m_count == 1)
	{
		m_isActive = !m_isActive;
	}

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
void CTextField::AddChar(const char c)
{
	if (m_cursorLocation == m_value->GetLength())
	{
		m_cursorLocation = m_value->GetLength() - 1;

		if (m_cursorLocation < 0)
		{
			m_cursorLocation = 0;
		}
	}

	m_value->GetText()[m_cursorLocation] = (char)c;

	m_cursorLocation++;
}

/*
*/
void CTextField::Backspace()
{
	m_cursorLocation--;

	if (m_cursorLocation < 0)
	{
		m_cursorLocation = 0;
	}

	m_value->GetText()[m_cursorLocation] = 0x00;
}

/*
*/
void CTextField::DisplayText()
{
	SIZE size = m_font->TextDimensions(m_value->GetText());

	CVertex2 position(m_image->m_position.p.x + 4.0f, m_image->m_position.p.y + (m_image->m_size.p.y / 2.0f) - (size.cy / 2.0f));

	if (m_mask)
	{
		wchar_t* mask = new wchar_t[m_value->GetLength() + 1]();

		for (int i = 0; i < strlen(m_value->GetText()); i++)
		{
			mask[i] = 'x';
		}

		m_font->Draw(mask, position, m_image->m_size, m_globalObjects->m_color->SunYellow);

		delete[] mask;

		return;
	}

	m_font->Draw(m_value->GetWText(), position, m_image->m_size, m_globalObjects->m_color->SunYellow);
}

/*
*/
CNetwork* CTextField::GetEvent()
{
	return m_network;
}

/*
*/
void CTextField::Inactive()
{
	if (m_globalObjects->m_mouse->m_keyMap[CMouseDevice::E_LMB].m_count == 1)
	{
		m_isActive = false;
	}

	if (m_isActive)
	{
		return;
	}

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
void CTextField::Record()
{
	if (m_image->m_box->CheckPointInBox(&m_globalObjects->m_mouse->m_position))
	{
		CTextField::Active();
	}
	else
	{
		CTextField::Inactive();
	}

	m_image->Draw();
}
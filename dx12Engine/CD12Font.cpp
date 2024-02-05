#include "CD12Font.h"

/*
*/
CD12Font::CD12Font()
{
	memset(this, 0x00, sizeof(CD12Font));
}

/*
*/
CD12Font::CD12Font(CErrorLog* errorLog, CVideoDevice* videoDevice, CWindow* window, CColor* color,
	const char* face, float pitch, DWRITE_FONT_WEIGHT weight)
{
	memset(this, 0x00, sizeof(CD12Font));

	m_errorLog = errorLog;

	m_videoDevice = videoDevice;

	m_window = window;

	m_color = color;

	m_face = new CString(face);


	WCHAR localName[LOCALE_NAME_MAX_LENGTH];

	GetSystemDefaultLocaleName(localName, LOCALE_NAME_MAX_LENGTH);

	HRESULT hr = m_videoDevice->m_d3d11On12->m_dWriteFactory->CreateTextFormat(m_face->GetWText(),
		nullptr,
		weight,
		DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
		pitch,
		localName,
		&m_textFormat);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CD12Font::CD12Font::CreateTextFormat:%s:%s %s\n",
			face,
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());

		return;
	}

	m_height = m_textFormat->GetFontSize();

	hr = m_videoDevice->m_d3d11On12->m_d2dDeviceContext->CreateSolidColorBrush(
		D2D1::ColorF(m_color->White->m_RGBA[0], m_color->White->m_RGBA[1], m_color->White->m_RGBA[2], m_color->White->m_RGBA[3]),
		&m_textBrush);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CD12Font::CD12Font::CreateSolidColorBrush:%s:%s %s\n",
			face,
			m_errorLog->GetDXGIErrorMessage(hr)->m_name->GetText(),
			m_errorLog->GetDXGIErrorMessage(hr)->m_msg->GetText());
	}
}

/*
*/
CD12Font::~CD12Font()
{
	m_textBrush.Reset();

	m_textFormat.Reset();

	delete m_face;
}

/*
*/
void CD12Font::Draw(const wchar_t* text, CVertex2 position, CVertex2 size, CColor* color)
{
	m_textRect = D2D1::RectF(position.p.x, position.p.y, position.p.x + size.p.x, position.p.y + size.p.y);

	m_textBrush->SetColor(D2D1::ColorF(color->m_RGBA[0], color->m_RGBA[1], color->m_RGBA[2], color->m_RGBA[3]));

	if (m_textFormat && m_textBrush)
	{
		m_videoDevice->m_d3d11On12->m_d2dDeviceContext->DrawText(text,
			(UINT32)wcslen(text),
			m_textFormat.Get(),
			&m_textRect,
			m_textBrush.Get());
	}
}

/*
*/
SIZE CD12Font::TextDimensions(const char* text)
{
	SIZE size = {};

	HDC hdc = GetDC(m_window->m_hWnd);

	// This matches the CD12Font 
	HFONT hFont = CreateFont(
		-MulDiv((int)m_height, GetDeviceCaps(hdc, LOGPIXELSY), (int)m_window->GetDpi()), //calculate the actual cHeight.
		0, 0, 0, // normal orientation
		m_textFormat->GetFontWeight(),   // normal weight--e.g., bold would be FW_BOLD
		false, false, false, // not italic, underlined or strike out
		DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, // select only outline (not bitmap) fonts
		CLIP_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, VARIABLE_PITCH, m_face->GetText());

	SelectObject(hdc, hFont);

	GetTextExtentPoint32(hdc, text, (int)strlen(text), &size);

	DeleteObject(hFont); //always delete the object after creating it.
	ReleaseDC(m_window->m_hWnd, hdc); //alway reelase dc after using.

	return size;
}
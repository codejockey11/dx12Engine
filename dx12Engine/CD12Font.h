#pragma once

#include "standard.h"

#include "CColor.h"
#include "CErrorLog.h"
#include "CString.h"
#include "CVideoDevice.h"
#include "CVertex.h"
#include "CWindow.h"

class CD12Font
{
public:

	CString* m_face;
	
	ComPtr<IDWriteTextFormat> m_textFormat;
	
	float m_height;

	CD12Font();
	CD12Font(CErrorLog* errorLog, CVideoDevice* device, CWindow* window, CColor* color, const char* face, float pitch, DWRITE_FONT_WEIGHT weight);
	~CD12Font();

	void Draw(const wchar_t* text, CVertex2 position, CVertex2 size, CColor* color);

	SIZE TextDimensions(const char* text);

private:

	CColor* m_color;
	CErrorLog* m_errorLog;
	CVideoDevice* m_videoDevice;
	CWindow* m_window;
	
	ComPtr<ID2D1SolidColorBrush> m_textBrush;
	
	D2D1_RECT_F m_textRect;
};
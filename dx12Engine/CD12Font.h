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

	ComPtr<IDWriteTextFormat> m_textFormat;

	float m_height;

	CString* m_face;


	CD12Font();

	CD12Font(CErrorLog* errorLog, CVideoDevice* device, CWindow* window, const char* face, float pitch, DWRITE_FONT_WEIGHT weight);

	~CD12Font();


	void Draw(const char* text, CVertex2 position, CVertex2 size, CColor* color);

	SIZE TextDimensions(const char* text);

private:

	CErrorLog* m_errorLog;

	CVideoDevice* m_videoDevice;

	CWindow* m_window;

	ComPtr<ID2D1SolidColorBrush> m_textBrush;

	D2D1_RECT_F m_textRect;
};
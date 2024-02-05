#pragma once

#include "standard.h"

#include "CColor.h"
#include "CD12Font.h"
#include "CLinkList.h"
#include "CVideoDevice.h"
#include "CWindow.h"

class CD12FontManager
{
public:

	CLinkList<CD12Font>* m_fonts;

	CD12FontManager();
	CD12FontManager(CVideoDevice* videoDevice, CErrorLog* errorLog, CWindow* window, CColor* color);
	~CD12FontManager();

	CD12Font* Create(const char* name, float pitch, DWRITE_FONT_WEIGHT weight);
	void Delete(const char* name);
	CD12Font* Get(const char* name);

private:

	CColor* m_color;
	CErrorLog* m_errorLog;
	CVideoDevice* m_videoDevice;
	CWindow* m_window;
};
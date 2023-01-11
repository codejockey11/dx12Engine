#pragma once

#include "standard.h"

#include "CD12Font.h"
#include "CLinkList.h"
#include "CVideoDevice.h"
#include "CWindow.h"

class CD12FontManager
{
public:

	CLinkList<CD12Font>* m_fonts;


	CD12FontManager();

	CD12FontManager(CErrorLog* errorLog, CVideoDevice* videoDevice, CWindow* window);

	~CD12FontManager();


	CD12Font* Create(const char* name, float pitch, DWRITE_FONT_WEIGHT weight);

	CD12Font* Get(const char* name);

	void Delete(const char* name);

private:

	CErrorLog* m_errorLog;

	CVideoDevice* m_videoDevice;

	CWindow* m_window;
};
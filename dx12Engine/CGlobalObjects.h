#pragma once

#include "standard.h"

#include "CColor.h"
#include "CConfiguration.h"
#include "CD12FontManager.h"
#include "CErrorLog.h"
#include "CKeyboardDevice.h"
#include "CModelManager.h"
#include "CMouseDevice.h"
#include "CShaderManager.h"
#include "CSoundDevice.h"
#include "CSoundManager.h"
#include "CString.h"
#include "CTextureManager.h"
#include "CVideoDevice.h"
#include "CWavefrontManager.h"
#include "CWindow.h"

class CGlobalObjects
{
public:

	CColor* m_color;
	CConfiguration* m_config;
	CD12FontManager* m_fontD12Mgr;
	CErrorLog* m_errorLog;
	CKeyboardDevice* m_keyboard;
	CModelManager* m_modelManager;
	CMouseDevice* m_mouse;
	CShaderManager* m_shaderMgr;
	CSoundDevice* m_soundDevice;
	CSoundManager* m_soundMgr;
	CString* m_className;
	CString* m_title;
	CTextureManager* m_textureMgr;
	CVideoDevice* m_videoDevice;
	CWavefrontManager* m_wavefrontManager;
	CWindow* m_window;

	CGlobalObjects();
	CGlobalObjects(HINSTANCE hInstance, const char* className, const char* title, WNDPROC wndProc, BYTE type, const char* logFilename, const char* configFilename);
	~CGlobalObjects();
};
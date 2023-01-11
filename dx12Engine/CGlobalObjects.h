#pragma once

#include "standard.h"

#include "CConfiguration.h"
#include "CD12FontManager.h"
#include "CErrorLog.h"
#include "CFrametime.h"
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

	CErrorLog* m_errorLog;
	CConfiguration* m_config;
	CD12FontManager* m_fontD12Mgr;
	CFrametime* m_frametime;
	CKeyboardDevice* m_keyboard;
	CModelManager* m_modelManager;
	CMouseDevice* m_mouse;
	CShaderManager* m_shaderMgr;
	CSoundDevice* m_soundDevice;
	CSoundManager* m_soundMgr;
	CTextureManager* m_textureMgr;
	CWavefrontManager* m_wavefrontManager;
	CVideoDevice* m_videoDevice;
	CWindow* m_window;

	CString* m_className;
	CString* m_title;


	CGlobalObjects();

	CGlobalObjects(HINSTANCE hInstance, const char* className, const char* title, WNDPROC wndProc, BYTE type, const char* logFilename, const char* configFilename);

	~CGlobalObjects();
};
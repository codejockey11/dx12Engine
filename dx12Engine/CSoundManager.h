#pragma once

#include "standard.h"

#include "CErrorLog.h"
#include "CLinkList.h"
#include "CSound.h"
#include "CSoundDevice.h"

class CSoundManager
{
public:

	CSoundManager();

	CSoundManager(CErrorLog* errorLog, CSoundDevice* soundDevice);

	~CSoundManager();


	CSound* MakeSound(const char* filename, BYTE loop);

	CSound* GetSound(const char* filename);

private:

	CErrorLog* m_errorLog;

	CSoundDevice* m_soundDevice;

	CLinkList<CSound>* m_sounds;
};
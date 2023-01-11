#include "CSoundManager.h"

CSoundManager::CSoundManager()
{
	memset(this, 0x00, sizeof(CSoundManager));
}

CSoundManager::CSoundManager(CErrorLog* errorLog, CSoundDevice* soundDevice)
{
	memset(this, 0x00, sizeof(CSoundManager));

	m_errorLog = errorLog;

	m_soundDevice = soundDevice;

	m_sounds = new CLinkList<CSound>();
}

CSoundManager::~CSoundManager()
{
	delete m_sounds;
}

CSound* CSoundManager::MakeSound(const char* filename, BYTE loop)
{
	CSound* sound = CSoundManager::GetSound(filename);

	if (sound)
	{
		return sound;
	}

	sound = new CSound(m_errorLog, m_soundDevice, filename, loop);

	if (sound->m_initialized)
	{
		m_sounds->Add(sound, filename);

		return sound;
	}

	m_errorLog->WriteError("CSoundManager::MakeSound:Failed:%s\n", filename);

	delete sound;

	return nullptr;
}

CSound* CSoundManager::GetSound(const char* filename)
{
	CLinkListNode<CSound>* lln = m_sounds->Search(filename);

	if (lln)
	{
		return lln->m_element;
	}

	return nullptr;
}
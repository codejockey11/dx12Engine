#pragma once

#include "standard.h"

#include "CErrorLog.h"

class CSoundDevice
{
public:

	IXAudio2* m_xAudio2;

	X3DAUDIO_HANDLE m_3DAudio;

	IXAudio2MasteringVoice* m_masteringVoice;

	DWORD m_channelMask;


	CSoundDevice();

	CSoundDevice(CErrorLog* errorLog);

	~CSoundDevice();

private:

	CErrorLog* m_errorLog;

	UINT32 m_count;

	IMMDeviceEnumerator* m_pEnumerator;
	IMMDeviceCollection* m_pCollection;
	IMMDevice* m_pEndpoint;

	IPropertyStore* m_pProps;

	LPWSTR m_pwszID;

	PROPVARIANT m_varName;

	CLSID m_CLSID_MMDeviceEnumerator;

	IID m_IID_IMMDeviceEnumerator;
};
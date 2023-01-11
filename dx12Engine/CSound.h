#pragma once

#include "standard.h"

#include "CErrorLog.h"
#include "CSoundDevice.h"
#include "CString.h"
#include "CWavLoader.h"

class CSound
{
public:

	enum
	{
		SE_REVERB = 0,
		SE_ECHO,
		SE_REVERBXAPO,

		EFFECT_COUNT = 3,

		SOURCE_VOICE_COUNT = 16
	};

	CString* m_filename;

	BYTE m_initialized;


	CSound();

	CSound(CErrorLog* errorLog, CSoundDevice* soundDevice, const char* filename, bool loop);

	~CSound();


	void SetEffectParameters();

	void ToggleEffect(BYTE soundEffect);

	void StartSound();

	void StopSound();

	void Shutdown();

	bool IsPlaying();

private:

	CErrorLog* m_errorLog;

	CSoundDevice* m_soundDevice;

	CWavLoader* m_wavLoader;

	XAUDIO2_EFFECT_DESCRIPTOR m_effectDesc[CSound::EFFECT_COUNT];

	XAUDIO2_EFFECT_CHAIN m_effectChain;

	IXAudio2SourceVoice* m_sourceVoice[CSound::SOURCE_VOICE_COUNT];

	XAUDIO2_BUFFER m_buffer;

	XAUDIO2_VOICE_STATE m_voiceState;

	BYTE m_isOn[CSound::EFFECT_COUNT];

	IUnknown* m_xapoEffect[CSound::EFFECT_COUNT];

	XAUDIO2FX_REVERB_PARAMETERS	m_reverbParameters;

	FXECHO_PARAMETERS m_xapoEcho;

	FXREVERB_PARAMETERS m_xapoReverb;


	void SetEffectChain();
};
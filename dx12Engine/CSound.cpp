#include "CSound.h"

/*
*/
CSound::CSound()
{
	memset(this, 0x00, sizeof(CSound));
}

/*
*/
CSound::CSound(CErrorLog* errorLog, CSoundDevice* soundDevice, const char* filename, bool loop)
{
	memset(this, 0x00, sizeof(CSound));

	m_errorLog = errorLog;

	m_soundDevice = soundDevice;

	m_filename = new CString(filename);

	m_wavLoader = new CWavLoader(errorLog, filename);

	for (UINT i = 0; i < CSound::E_SOURCE_VOICE_COUNT; i++)
	{
		HRESULT hr = m_soundDevice->m_xAudio2->CreateSourceVoice(&m_sourceVoice[i], &m_wavLoader->m_wfx);

		if (hr != S_OK)
		{
			m_errorLog->WriteError("CSound::CSound::CreateSourceVoice:%s\n", m_errorLog->GetComErrorMessage(hr));

			return;
		}
	}

	m_buffer.AudioBytes = m_wavLoader->m_header.m_subchunk2Size;
	m_buffer.Flags = XAUDIO2_END_OF_STREAM;
	m_buffer.pAudioData = m_wavLoader->m_data;

	if (loop)
	{
		m_buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	}


	HRESULT hr = CreateFX(__uuidof(FXEcho), &m_xapoEffect[CSound::E_SE_ECHO]);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CSound::CSound::CreateFX::FXEcho:%s\n", m_errorLog->GetComErrorMessage(hr));

		return;
	}

	// Princeton Digital Reverb
	hr = XAudio2CreateReverb(&m_xapoEffect[CSound::E_SE_REVERB]);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CSound::CSound::XAudio2CreateReverb:%s\n", m_errorLog->GetComErrorMessage(hr));

		return;
	}

	hr = CreateFX(__uuidof(FXReverb), &m_xapoEffect[CSound::E_SE_REVERBXAPO]);

	if (hr != S_OK)
	{
		m_errorLog->WriteError("CSound::CSound::CreateFX::FXReverb:%s\n", m_errorLog->GetComErrorMessage(hr));

		return;
	}


	m_xapoEcho.Delay = 250.0f;	// FXECHO_DEFAULT_DELAY;
	m_xapoEcho.Feedback = 0.50f;	// FXECHO_DEFAULT_FEEDBACK;
	m_xapoEcho.WetDryMix = 0.50f;	// FXECHO_DEFAULT_WETDRYMIX;

	m_effectDesc[CSound::E_SE_ECHO].InitialState = false;
	m_effectDesc[CSound::E_SE_ECHO].OutputChannels = m_wavLoader->m_wfx.nChannels;
	m_effectDesc[CSound::E_SE_ECHO].pEffect = m_xapoEffect[CSound::E_SE_ECHO];


	m_reverbParameters.ReflectionsDelay = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_DELAY;
	m_reverbParameters.ReverbDelay = XAUDIO2FX_REVERB_DEFAULT_REVERB_DELAY;
	m_reverbParameters.RearDelay = XAUDIO2FX_REVERB_DEFAULT_REAR_DELAY;
	m_reverbParameters.PositionLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	m_reverbParameters.PositionRight = XAUDIO2FX_REVERB_DEFAULT_POSITION;
	m_reverbParameters.PositionMatrixLeft = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	m_reverbParameters.PositionMatrixRight = XAUDIO2FX_REVERB_DEFAULT_POSITION_MATRIX;
	m_reverbParameters.EarlyDiffusion = XAUDIO2FX_REVERB_DEFAULT_EARLY_DIFFUSION;
	m_reverbParameters.LateDiffusion = XAUDIO2FX_REVERB_DEFAULT_LATE_DIFFUSION;
	m_reverbParameters.LowEQGain = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_GAIN;
	m_reverbParameters.LowEQCutoff = XAUDIO2FX_REVERB_DEFAULT_LOW_EQ_CUTOFF;
	m_reverbParameters.HighEQGain = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_GAIN;
	m_reverbParameters.HighEQCutoff = XAUDIO2FX_REVERB_DEFAULT_HIGH_EQ_CUTOFF;
	m_reverbParameters.RoomFilterFreq = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_FREQ;
	m_reverbParameters.RoomFilterMain = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_MAIN;
	m_reverbParameters.RoomFilterHF = XAUDIO2FX_REVERB_DEFAULT_ROOM_FILTER_HF;
	m_reverbParameters.ReflectionsGain = XAUDIO2FX_REVERB_DEFAULT_REFLECTIONS_GAIN;
	m_reverbParameters.ReverbGain = XAUDIO2FX_REVERB_DEFAULT_REVERB_GAIN;
	m_reverbParameters.DecayTime = XAUDIO2FX_REVERB_DEFAULT_DECAY_TIME;
	m_reverbParameters.Density = XAUDIO2FX_REVERB_DEFAULT_DENSITY;
	m_reverbParameters.RoomSize = XAUDIO2FX_REVERB_DEFAULT_ROOM_SIZE;
	m_reverbParameters.WetDryMix = XAUDIO2FX_REVERB_DEFAULT_WET_DRY_MIX;

	//XAUDIO2FX_REVERB_I3DL2_PARAMETERS ppp = XAUDIO2FX_I3DL2_PRESET_STONEROOM;
	//ReverbConvertI3DL2ToNative(&ppp, &m_reverbParameters);

	m_effectDesc[CSound::E_SE_REVERB].InitialState = false;
	m_effectDesc[CSound::E_SE_REVERB].OutputChannels = m_wavLoader->m_wfx.nChannels;
	m_effectDesc[CSound::E_SE_REVERB].pEffect = m_xapoEffect[CSound::E_SE_REVERB];


	m_xapoReverb.Diffusion = FXREVERB_DEFAULT_DIFFUSION;
	m_xapoReverb.RoomSize = FXREVERB_DEFAULT_ROOMSIZE;

	m_effectDesc[CSound::E_SE_REVERBXAPO].InitialState = false;
	m_effectDesc[CSound::E_SE_REVERBXAPO].OutputChannels = m_wavLoader->m_wfx.nChannels;
	m_effectDesc[CSound::E_SE_REVERBXAPO].pEffect = m_xapoEffect[CSound::E_SE_REVERBXAPO];


	m_effectChain.EffectCount = CSound::E_EFFECT_COUNT;
	m_effectChain.pEffectDescriptors = m_effectDesc;

	CSound::SetEffectChain();
	CSound::SetEffectParameters();

	//CSound::ToggleEffect(CSound::E_SE_ECHO);
	//CSound::ToggleEffect(CSound::E_SE_REVERB);
	//CSound::ToggleEffect(CSound::E_SE_REVERBXAPO);

	m_initialized = true;
}

/*
*/
CSound::~CSound()
{
	CSound::StopSound();
	CSound::Shutdown();

	delete m_filename;
	delete m_wavLoader;

	if (m_xapoEffect[CSound::E_SE_REVERB])
	{
		m_xapoEffect[CSound::E_SE_REVERB]->Release();
	}

	if (m_xapoEffect[CSound::E_SE_ECHO])
	{
		m_xapoEffect[CSound::E_SE_ECHO]->Release();
	}

	if (m_xapoEffect[CSound::E_SE_REVERBXAPO])
	{
		m_xapoEffect[CSound::E_SE_REVERBXAPO]->Release();
	}
}

/*
*/
bool CSound::IsPlaying()
{
	for (UINT i = 0; i < CSound::E_SOURCE_VOICE_COUNT; i++)
	{
		m_sourceVoice[i]->GetState(&m_voiceState);

		if (m_voiceState.BuffersQueued == 1)
		{
			return true;
		}
	}

	return false;
}

/*
*/
void CSound::SetEffectChain()
{
	for (UINT i = 0; i < CSound::E_SOURCE_VOICE_COUNT; i++)
	{
		HRESULT hr = m_sourceVoice[i]->SetEffectChain(&m_effectChain);

		if (hr != S_OK)
		{
			m_errorLog->WriteError("CSound::SetEffectChain::SetEffectChain:%s\n", m_errorLog->GetComErrorMessage(hr));

			return;
		}
	}
}

/*
*/
void CSound::SetEffectParameters()
{
	for (UINT i = 0; i < CSound::E_SOURCE_VOICE_COUNT; i++)
	{
		HRESULT hr = m_sourceVoice[i]->SetEffectParameters(CSound::E_SE_REVERB,
			&m_reverbParameters,
			sizeof(XAUDIO2FX_REVERB_PARAMETERS),
			XAUDIO2_COMMIT_NOW);

		if (hr != S_OK)
		{
			m_errorLog->WriteError("CSound::SetEffectParameters:CSound::SE_REVERB:%s\n", m_errorLog->GetComErrorMessage(hr));

			return;
		}

		hr = m_sourceVoice[i]->SetEffectParameters(CSound::E_SE_ECHO, &m_xapoEcho, sizeof(FXECHO_PARAMETERS), XAUDIO2_COMMIT_NOW);

		if (hr != S_OK)
		{
			m_errorLog->WriteError("CSound::SetEffectParameters:CSound::SE_ECHO:%s\n", m_errorLog->GetComErrorMessage(hr));

			return;
		}

		hr = m_sourceVoice[i]->SetEffectParameters(CSound::E_SE_REVERBXAPO, &m_xapoReverb, sizeof(FXREVERB_PARAMETERS), XAUDIO2_COMMIT_NOW);

		if (hr != S_OK)
		{
			m_errorLog->WriteError("CSound::SetEffectParameters:CSound::SE_REVERBXAPO:%s\n", m_errorLog->GetComErrorMessage(hr));

			return;
		}
	}
}

/*
*/
void CSound::Shutdown()
{
	for (UINT i = 0; i < CSound::E_SOURCE_VOICE_COUNT; i++)
	{
		HRESULT hr = m_sourceVoice[i]->FlushSourceBuffers();

		m_sourceVoice[i]->DestroyVoice();
	}
}

/*
*/
void CSound::StartSound()
{
	for (UINT i = 0; i < CSound::E_SOURCE_VOICE_COUNT; i++)
	{
		m_sourceVoice[i]->GetState(&m_voiceState);

		if (m_voiceState.BuffersQueued == 1)
		{
		}
		else
		{
			HRESULT hr = m_sourceVoice[i]->SubmitSourceBuffer(&m_buffer);

			if (hr != S_OK)
			{
				m_errorLog->WriteError("CSound::StartSound::SubmitSourceBuffer:%s\n", m_errorLog->GetComErrorMessage(hr));

				return;
			}

			hr = m_sourceVoice[i]->Start(0);

			if (hr != S_OK)
			{
				m_errorLog->WriteError("CSound::StartSound::Start:%s\n", m_errorLog->GetComErrorMessage(hr));

				return;
			}

			break;
		}
	}
}

/*
*/
void CSound::StopSound()
{
	for (UINT i = 0; i < CSound::E_SOURCE_VOICE_COUNT; i++)
	{
		HRESULT hr = m_sourceVoice[i]->Stop(0);

		if (hr != S_OK)
		{
			m_errorLog->WriteError("CSound::StopSound::Stop:%s\n", m_errorLog->GetComErrorMessage(hr));

			return;
		}
	}
}

/*
*/
void CSound::ToggleEffect(BYTE soundEffect)
{
	if (m_isOn[soundEffect])
	{
		m_isOn[soundEffect] = false;

		for (UINT i = 0; i < CSound::E_SOURCE_VOICE_COUNT; i++)
		{
			HRESULT hr = m_sourceVoice[i]->DisableEffect(soundEffect);

			if (hr != S_OK)
			{
				m_errorLog->WriteError("CSound::ToggleEffect::DisableEffect:%s\n", m_errorLog->GetComErrorMessage(hr));

				return;
			}
		}
	}
	else
	{
		m_isOn[soundEffect] = true;

		for (UINT i = 0; i < CSound::E_SOURCE_VOICE_COUNT; i++)
		{
			HRESULT hr = m_sourceVoice[i]->EnableEffect(soundEffect);

			if (hr != S_OK)
			{
				m_errorLog->WriteError("CSound::ToggleEffect::EnableEffect:%s\n", m_errorLog->GetComErrorMessage(hr));

				return;
			}
		}
	}
}
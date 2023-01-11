#pragma once

#include "standard.h"

#include "CErrorLog.h"
#include "CKeyMap.h"
#include "CEvent.h"
#include "CWindow.h"
#include "CVertex.h"

class CMouseDevice
{
public:

	enum
	{
		LMB = 0,
		MMB,
		RMB,
		SCR,

		KR_ONESHOT = 0,
		KR_REPEATER = 1,

		BUTTON_COUNT = 4
	};

	long m_lX;
	long m_lY;
	long m_lZ;

	BYTE m_rgbButtons[4];

	CVertex2 m_position;

	POINT m_physicalPosition;

	POINT m_prevPosition;

	bool m_positionSaved;

	POINT m_savePosition;

	CKeyMap* m_keyMap;

	float m_sensitivity;


	CMouseDevice();

	CMouseDevice(CErrorLog* errorLog, CWindow* window, float sensitivity);

	~CMouseDevice();


	void GetState();

	void SetKeyMap(UINT number, BYTE repeater, BYTE audience, BYTE type, BYTE data, int length);

	void MouseMove();

	void MouseClick(int button, int value);

	void MouseWheel(long value);

	void FirstPersonReset();

	void SaveLastPosition();

	void SetLastPosition();

private:

	CErrorLog* m_errorLog;

	CWindow* m_window;
};
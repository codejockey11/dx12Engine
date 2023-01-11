#pragma once

#include "standard.h"

#include "CKeyMap.h"
#include "CEvent.h"

class CKeyboardDevice
{
public:

	enum
	{
		KR_ONESHOT = 1,
		KR_REPEATER,

		KEY_COUNT = 256,

		// undefined virtual keys equate to unshifted ascii value
		VK_A = 65,
		VK_D = 68,
		VK_S = 83,
		VK_W = 87,
		VK_EQUALS = 187
	};

	char m_keys[CKeyboardDevice::KEY_COUNT];

	CKeyMap* m_keyMap;


	CKeyboardDevice();

	~CKeyboardDevice();


	void GetState();

	void SetKeyMap(UINT number, BYTE repeater, BYTE audience, BYTE type, BYTE data, int length);

	void KeyDown(int key);

	void KeyUp(int key);
};
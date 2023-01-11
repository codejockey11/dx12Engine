#pragma once

#include "standard.h"

#include "CEvent.h"

class CKeyMap
{
public:

	UINT m_count;

	BYTE m_repeater;

	CEvent* m_event;

	CKeyMap();

	~CKeyMap();
};
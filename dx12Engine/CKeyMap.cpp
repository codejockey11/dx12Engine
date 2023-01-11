#include "CKeyMap.h"

CKeyMap::CKeyMap()
{
	memset(this, 0x00, sizeof(CKeyMap));
}

CKeyMap::~CKeyMap()
{
	if (m_event)
	{
		delete m_event;
	}
}
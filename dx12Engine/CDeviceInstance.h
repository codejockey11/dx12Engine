#pragma once

#include "standard.h"

#include "CString.h"

class CDeviceInstance
{
public:

	CString* m_instanceName;

	GUID m_guidInstance;

	CDeviceInstance();
	CDeviceInstance(GUID* g, const char* c);
	~CDeviceInstance();
};
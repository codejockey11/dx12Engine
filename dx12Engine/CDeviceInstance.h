#pragma once

#include "standard.h"

#include "CString.h"

class CDeviceInstance
{
public:

	CString* instanceName;

	GUID guidInstance;


	CDeviceInstance();

	CDeviceInstance(GUID* g, const char* c);

	~CDeviceInstance();
};
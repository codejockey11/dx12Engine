#include "CDeviceInstance.h"

CDeviceInstance::CDeviceInstance()
{
	memset(this, 0x00, sizeof(CDeviceInstance));
}

CDeviceInstance::CDeviceInstance(GUID* g, const char* c)
{
	memset(this, 0x00, sizeof(CDeviceInstance));

	memcpy((void*)&guidInstance, (void*)g, sizeof(GUID));

	instanceName = new CString(c);
}

CDeviceInstance::~CDeviceInstance()
{
	delete instanceName;
}
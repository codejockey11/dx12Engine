#include "CDeviceInstance.h"

/*
*/
CDeviceInstance::CDeviceInstance()
{
	memset(this, 0x00, sizeof(CDeviceInstance));
}

/*
*/
CDeviceInstance::CDeviceInstance(GUID* guid, const char* name)
{
	memset(this, 0x00, sizeof(CDeviceInstance));

	memcpy_s((void*)&m_guidInstance, sizeof(GUID), (void*)guid, sizeof(GUID));

	m_instanceName = new CString(name);
}

/*
*/
CDeviceInstance::~CDeviceInstance()
{
	delete m_instanceName;
}
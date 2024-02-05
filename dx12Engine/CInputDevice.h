#ifndef _CInputDevice
#define _CInputDevice

#include "standard.h"

#include "CDeviceInstance.h"
#include "CErrorLog.h"
#include "CLinkList.h"

class CInputDevice
{
public:

	IDirectInput8* dinput;

	CLinkList<CDeviceInstance>* devices;

	CInputDevice();

	CInputDevice(CErrorLog* el);

	~CInputDevice();

private:

	HRESULT hr;

	CErrorLog* errorLog;
};
#endif
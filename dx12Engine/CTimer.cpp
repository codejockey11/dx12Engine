#include "CTimer.h"

CTimer::CTimer()
{
	memset(this, 0x00, sizeof(CTimer));
}

CTimer::CTimer(ULONGLONG d)
{
	memset(this, 0x00, sizeof(CTimer));

	duration = d;
}

CTimer::~CTimer()
{
}

VOID CTimer::Start()
{
	if (isReloading == TRUE)
	{
		return;
	}

	isReloading = TRUE;

	currTime = GetTickCount64();

	lastTime = currTime;
}

VOID CTimer::Frame()
{
	currTime = GetTickCount64();

	if ((currTime - lastTime) > duration)
	{
		lastTime = currTime;

		isReloading = FALSE;
	}
}
#ifndef _CTIMER
#define _CTIMER

#include "standard.h"

class CTimer
{
public:

	ULONGLONG currTime;
	
	ULONGLONG lastTime;

	ULONGLONG duration;

	BOOL isReloading;

	CTimer();

	CTimer(ULONGLONG d);

	~CTimer();

	VOID Start();

	VOID Frame();
};
#endif
#pragma once

#include "standard.h"

class CFrametime
{
public:

	float m_frametime;

	int m_rframecount;

	float m_rframetime;

	ULONGLONG m_currTime;

	UINT64 m_counter;


	CFrametime();

	~CFrametime();


	void Frame();

private:

	ULONGLONG m_startTime;

	ULONGLONG m_countTime;

	int m_framecount;
};
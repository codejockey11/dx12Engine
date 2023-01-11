#pragma once

#include "standard.h"

#include "CClientSocket.h"
#include "CCollision.h"
#include "CEvent.h"
#include "CFrametime.h"
#include "CServerEnvironment.h"
#include "CString.h"
#include "CVertex.h"

class CPlayerInfo
{
public:

	enum
	{
		PT_RED = 0,
		PT_BLUE,
		PT_GREEN,
		PT_YELLOW,

		PT_ACTIVE
	};

	char m_name[32];

	BYTE m_team;

	CVertex m_position;
	CVertex m_direction;

	BYTE m_state;

	int m_number;

	bool m_isReady;

	CServerEnvironment* m_environment;

	CFrametime* m_frametime;

	CCollision* m_collision;


	CPlayerInfo();

	~CPlayerInfo();


	void PlayerMove();

	bool GroundCollision();
};
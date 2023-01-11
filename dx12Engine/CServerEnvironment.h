#pragma once

#include "standard.h"

#include "CHeapArray.h"
#include "CTeamStart.h"
#include "CTerrainClip.h"
#include "CTerrainTile.h"

class CServerEnvironment
{
public:

	enum
	{
		MAX_PLAYERS = 5
	};

	int m_width;
	int m_height;
	int m_primSize;

	CHeapArray* m_clips;

	CTeamStart* m_redTeamStart[CServerEnvironment::MAX_PLAYERS];

	CTeamStart* m_blueTeamStart[CServerEnvironment::MAX_PLAYERS];


	CServerEnvironment();

	CServerEnvironment(const char* filename);

	~CServerEnvironment();


};


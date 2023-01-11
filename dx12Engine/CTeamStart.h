#pragma once

#include "standard.h"

#include "CVertex.h"

class CTeamStart
{
public:

	bool m_isAvailable;

	CVertex m_position;

	CVertex m_direction;

	
	CTeamStart();

	CTeamStart(bool isAvailable, CVertex position, CVertex direction);

	~CTeamStart();
};


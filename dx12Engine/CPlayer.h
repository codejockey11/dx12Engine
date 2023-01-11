#pragma once

#include "standard.h"

#include "CCamera.h"
#include "CCollision.h"
#include "CGlobalObjects.h"
#include "CHeapArray.h"
#include "CEvent.h"
#include "CTeamStart.h"
#include "CTerrain.h"

class CPlayer
{
public:

	enum
	{
		PT_RED = 0,
		PT_BLUE,
		PT_GREEN,
		PT_YELLOW
	};

	CCamera* m_camera;
	
	CCollision* m_collision;

	CVertex2 m_coordinates;
	
	CVertex m_position;
	
	CVertex m_direction;

	CString* m_name;

	BYTE m_team;

	CTerrain* m_terrain;

	
	CPlayer();

	CPlayer(CGlobalObjects* globalObjects, CVertex* position, CVertex* direction, CTerrain* terrain, BYTE team, float rate, const char* name);

	~CPlayer();

	
	void PlayerMove(CEvent* evt);

private:

	CGlobalObjects* m_globalObjects;

	float m_rate;
	
	float m_distance;

	bool GroundCollision();
};
#pragma once

#include "standard.h"

#include "CErrorLog.h"
#include "CGlobalObjects.h"
#include "COrthoLevel.h"
#include "CEvent.h"
#include "CImage.h"

class COrthoPlayer
{
public:

	CVertex2 m_coordinates;
	CVertex2 m_position;
	CVertex2 m_direction;

	CString* m_name;

	CImage* m_image;

	CVertex2 m_cellSize;

	CVertex2 m_scaleSize;

	
	COrthoPlayer();

	COrthoPlayer(CGlobalObjects* globalObjects, COrthoLevel* orthoLevel, float rate, CVertex2* cellSize, CVertex2* sheetSize, CVertex2* cellPosition, const char* name);

	~COrthoPlayer();

	
	void PlayerMove(CEvent* evt);

	BYTE CheckLevelCollision();

private:

	CGlobalObjects* m_globalObjects;

	COrthoLevel* m_level;

	float m_rate;

	float m_distance;
};
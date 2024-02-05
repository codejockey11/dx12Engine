#pragma once

#include "standard.h"

#include "CGlobalObjects.h"
#include "CHeapArray.h"
#include "CLinkList.h"
#include "CString.h"
#include "CObject.h"
#include "CTeamStart.h"
#include "CTerrainCollision.h"
#include "CTerrainTile.h"

class CServerEnvironment
{
public:

	CHeapArray* m_collectables;
	CHeapArray* m_collisions;
	CLinkList<CTeamStart>* m_redTeamStarts;
	CLinkList<CTeamStart>* m_blueTeamStarts;
	CString* m_name;
	
	int m_px;
	int m_pz;

	UINT m_gridHeight;
	UINT m_gridUnits;
	UINT m_gridWidth;
	UINT m_height;
	UINT m_primSize;
	UINT m_width;

	CServerEnvironment();
	CServerEnvironment(CGlobalObjects* globalObjects, const char* filename);
	~CServerEnvironment();

private:

	CGlobalObjects* m_globalObjects;
};
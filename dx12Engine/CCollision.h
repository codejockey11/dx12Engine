#pragma once

#include "standard.h"

#include "CFrametime.h"
#include "CServerInfo.h"
#include "CServerEnvironment.h"
#include "CTimer.h"
#include "CVertex.h"

class CCollision
{
public:

	CFrametime* m_frametime;
	CServerEnvironment* m_environment;
	CServerInfo* m_serverInfo;

	float m_length;
	float m_u;
	float m_v;

	CCollision();
	~CCollision();

	bool GroundCollision();
	bool IntersectPlane(CVertex* planeNormal, CVertex* planeOrigin, CVertex* rayOrigin, CVertex* rayDirection);
	void PlayerMove(CServerInfo* serverInfo);
	bool RayTriangleIntersect(CVertex* P, CVertex* A, CVertex* B, CVertex* C);
};
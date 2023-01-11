#pragma once

#include "standard.h"

#include "CVertex.h"

class CCollision
{
public:

	float m_length;
	float m_u;
	float m_v;

	CCollision();

	~CCollision();

	bool RayTriangleIntersect(CVertex* P, CVertex* A, CVertex* B, CVertex* C);

	bool IntersectPlane(CVertex* planeNormal, CVertex* planeOrigin, CVertex* rayOrigin, CVertex* rayDirection);
};
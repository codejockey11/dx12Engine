#include "CCollision.h"

CCollision::CCollision()
{
	memset(this, 0x00, sizeof(CCollision));
}

CCollision::~CCollision()
{
}

bool CCollision::IntersectPlane(CVertex* planeNormal, CVertex* planeOrigin, CVertex* rayOrigin, CVertex* rayDirection)
{
	// Assuming vectors planeNormal and rayDirection are normalized
	float denom = planeNormal->Dot(*rayDirection);

	// Determine if ray is parallel or pointing away from the plane
	if (denom > -0.0f)
	{
		return false;
	}

	CVertex p0l0 = *planeOrigin - *rayOrigin;

	// To keep translation don't normalize p0l0
	m_length = p0l0.Dot(*planeNormal) / denom;

	// Returning true when collision in front of starting point
	// No need to test return value unless it is important to the caller
	if (m_length >= 0.0f)
	{
		return true;
	}

	return false;
}

// blackpawn.com/texts/pointinpoly/default.html
// realtimecollisiondetection.net
// en.wikipedia.org/wiki/Barycentric_coordinate_system
// mathworld.wolfram.com/BarycentricCoordinates.html
bool CCollision::RayTriangleIntersect(CVertex* P, CVertex* A, CVertex* B, CVertex* C)
{
	// Compute vectors
	CVertex v0 = *C - *A;
	CVertex v1 = *B - *A;
	CVertex v2 = *P - *A;

	// Compute dot products
	float dot00 = v0.Dot(v0);
	float dot01 = v0.Dot(v1);
	float dot02 = v0.Dot(v2);
	float dot11 = v1.Dot(v1);
	float dot12 = v1.Dot(v2);

	// Compute barycentric coordinates
	float invDenom = 1 / ((dot00 * dot11) - (dot01 * dot01));

	// uv of intersection
	m_u = ((dot11 * dot02) - (dot01 * dot12)) * invDenom;
	m_v = ((dot00 * dot12) - (dot01 * dot02)) * invDenom;

	// Check if point is in triangle
	if ((m_u >= 0) && (m_v >= 0) && ((m_u + m_v) <= 1.0f))
	{
		return true;
	}

	return false;
}
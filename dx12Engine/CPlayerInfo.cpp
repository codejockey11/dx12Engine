#include "CPlayerInfo.h"

CPlayerInfo::CPlayerInfo()
{
	memset(this, 0x00, sizeof(CPlayerInfo));

	m_collision = new CCollision();
}

CPlayerInfo::~CPlayerInfo()
{
	delete m_collision;
}

void CPlayerInfo::PlayerMove()
{
	switch (m_state)
	{
	case CEvent::ET_FORWARD:
	{
		break;
	}
	case CEvent::ET_BACKWARD:
	{
		m_direction.p.x = m_direction.p.x * -1.0f;
		m_direction.p.y = m_direction.p.y * -1.0f;
		m_direction.p.z = m_direction.p.z * -1.0f;

		break;
	}
	case CEvent::ET_STRAFELEFT:
	{
		float yaw = -90.0F * PiDiv180;

		m_direction = m_direction.RotateAngleByAxis(yaw, CVertex(0.0f, 1.0f, 0.0f));

		break;
	}
	case CEvent::ET_STRAFERIGHT:
	{
		float yaw = 90.0F * PiDiv180;

		m_direction = m_direction.RotateAngleByAxis(yaw, CVertex(0.0f, 1.0f, 0.0f));

		break;
	}
	}

	m_direction.Normalize();

	if (m_environment)
	{
		CPlayerInfo::GroundCollision();
	}
}

/*
Ground Clip

B-C
| |
A-D

ABD has N1
DBC has N2
*/
bool CPlayerInfo::GroundCollision()
{
	CVertex newPosition;

	m_position.p.x += (m_frametime->m_frametime * 8.0f) * m_direction.p.x;
	m_position.p.y += (m_frametime->m_frametime * 8.0f) * m_direction.p.y;
	m_position.p.z += (m_frametime->m_frametime * 8.0f) * m_direction.p.z;

	// clusterSize represents a collection of clipping triangles for an
	// area of the terrain stored in a list.  This will determine the number of terrain nodes for
	// the width and height of the players immediate surrounding area.
	// For now ... clusterSize is just the size of two primitives that make one square tile for the terrain.
	// This clustersize will enable the use of another list that defines what groups will be drawn during rendering
	// based on what cluster the player is in.
	/*
	The top left cluster has four ground tiles within it and the terrain heap
	array will contain a link list that houses a group of those clips plus any others the cluster contains
	Cluster array is 3x3 so the heap array would be defined as a 3x3 link list and the elements would be the clipping triangles.
	----------
	|**|  |  |
	|**|  |  |
	----------
	|  |  |  |
	|  |  |  |
	----------
	|  |  |  |
	|  |  |  |
	----------
	*/

	// If the the heap array housed 2 tiles the cluster size would be prim size times two.  4x4 would be 4.
	float clusterSize = (float)m_environment->m_primSize;

	// Not width minus one here because the terrain clips extend to the edge vertices of the height map.
	int px = (int)((m_position.p.x + ((m_environment->m_width / 2.0f) * (clusterSize))) / (clusterSize));
	int pz = (int)((m_position.p.z + ((m_environment->m_height / 2.0f) * (clusterSize))) / (clusterSize));

	if ((px >= 0) && (pz >= 0))
	{
		CTerrainClip* terrainClip = (CTerrainClip*)m_environment->m_clips->GetElement(2, px, pz);

		CVertex direction = { 0.0f, -1.0f, 0.0f };

		CVertex position = CVertex(m_position.p.x, m_position.p.y, m_position.p.z);

		CVertex pointOnPlane;

		m_collision->IntersectPlane(&terrainClip->m_n1, &terrainClip->m_a, &position, &direction);

		if (m_collision->m_length < 2.0f)
		{
			pointOnPlane.p.x = m_position.p.x;
			pointOnPlane.p.y = m_position.p.y - m_collision->m_length;
			pointOnPlane.p.z = m_position.p.z;

			if (m_collision->RayTriangleIntersect(&pointOnPlane, &terrainClip->m_c, &terrainClip->m_b, &terrainClip->m_a))
			{
				m_position.p.x = pointOnPlane.p.x;
				m_position.p.y = pointOnPlane.p.y + 2.0f;
				m_position.p.z = pointOnPlane.p.z;

				return true;
			}
		}

		m_collision->IntersectPlane(&terrainClip->m_n2, &terrainClip->m_f, &position, &direction);

		pointOnPlane.p.x = m_position.p.x;
		pointOnPlane.p.y = m_position.p.y - m_collision->m_length;
		pointOnPlane.p.z = m_position.p.z;

		if (m_collision->m_length < 2.0f)
		{
			if (m_collision->RayTriangleIntersect(&pointOnPlane, &terrainClip->m_f, &terrainClip->m_e, &terrainClip->m_d))
			{
				m_position.p.x = pointOnPlane.p.x;
				m_position.p.y = pointOnPlane.p.y + 2.0f;
				m_position.p.z = pointOnPlane.p.z;

				return true;
			}
		}
	}

	return false;
}
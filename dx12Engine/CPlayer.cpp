#include "CPlayer.h"

CPlayer::CPlayer()
{
	memset(this, 0x00, sizeof(CPlayer));
}

CPlayer::CPlayer(CGlobalObjects* globalObjects, CVertex* position, CVertex* direction, CTerrain* terrain, BYTE team, float rate, const char* name)
{
	memset(this, 0x00, sizeof(CPlayer));

	m_globalObjects = globalObjects;

	m_terrain = terrain;

	m_team = team;

	m_rate = rate;

	m_name = new CString(name);

	m_camera = new CCamera((float)globalObjects->m_window->m_width, (float)globalObjects->m_window->m_height, 45.0f, 1.0f, 50000.0f);

	m_collision = new CCollision();

	m_position = *position;
	
	m_camera->m_position.x = m_position.p.x;
	m_camera->m_position.y = m_position.p.y;
	m_camera->m_position.z = m_position.p.z;

	m_camera->UpdateRotation(direction->p.x, direction->p.y, direction->p.z);
	m_camera->UpdateView();

	m_direction = CVertex(m_camera->m_look.x, m_camera->m_look.y, m_camera->m_look.z);
}

CPlayer::~CPlayer()
{
	delete m_collision;
	delete m_camera;
	delete m_name;
}

void CPlayer::PlayerMove(CEvent* evt)
{
	m_distance = m_rate * m_globalObjects->m_frametime->m_frametime;

	m_direction = CVertex(m_camera->m_look.x, m_camera->m_look.y, m_camera->m_look.z);

	switch (evt->m_type)
	{
	case CEvent::ET_FORWARD:
	{
		break;
	}

	case CEvent::ET_BACKWARD:
	{
		m_direction.p.x = m_camera->m_look.x * -1.0f;
		m_direction.p.y = m_camera->m_look.y * -1.0f;
		m_direction.p.z = m_camera->m_look.z * -1.0f;

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

	if (m_terrain)
	{
		CPlayer::GroundCollision();
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
bool CPlayer::GroundCollision()
{
	m_camera->m_position.x += (m_globalObjects->m_frametime->m_frametime * 8.0f) * m_camera->m_look.x;
	m_camera->m_position.y += (m_globalObjects->m_frametime->m_frametime * 8.0f) * m_camera->m_look.y;
	m_camera->m_position.z += (m_globalObjects->m_frametime->m_frametime * 8.0f) * m_camera->m_look.z;

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
	float clusterSize = (float)m_terrain->m_primSize;

	// Not width minus one here because the terrain clips extend to the edge vertices of the height map.
	int px = (int)((m_camera->m_position.x + ((m_terrain->m_width / 2.0f) * (clusterSize))) / (clusterSize));
	int pz = (int)((m_camera->m_position.z + ((m_terrain->m_height / 2.0f) * (clusterSize))) / (clusterSize));

	if ((px >= 0) && (pz >= 0))
	{
		CTerrainClip* terrainClip = (CTerrainClip*)m_terrain->m_clips->GetElement(2, px, pz);

		CVertex direction = { 0.0f, -1.0f, 0.0f };

		CVertex position = CVertex(m_camera->m_position.x, m_camera->m_position.y, m_camera->m_position.z);

		CVertex pointOnPlane;

		m_collision->IntersectPlane(&terrainClip->m_n1, &terrainClip->m_a, &position, &direction);

		if (m_collision->m_length < 2.0f)
		{
			pointOnPlane.p.x = m_camera->m_position.x;
			pointOnPlane.p.y = m_camera->m_position.y - m_collision->m_length;
			pointOnPlane.p.z = m_camera->m_position.z;

			if (m_collision->RayTriangleIntersect(&pointOnPlane, &terrainClip->m_c, &terrainClip->m_b, &terrainClip->m_a))
			{
				m_camera->m_position.x = pointOnPlane.p.x;
				m_camera->m_position.y = pointOnPlane.p.y + 2.0f;
				m_camera->m_position.z = pointOnPlane.p.z;

				return true;
			}
		}

		m_collision->IntersectPlane(&terrainClip->m_n2, &terrainClip->m_f, &position, &direction);

		pointOnPlane.p.x = m_camera->m_position.x;
		pointOnPlane.p.y = m_camera->m_position.y - m_collision->m_length;
		pointOnPlane.p.z = m_camera->m_position.z;

		if (m_collision->m_length < 2.0f)
		{
			if (m_collision->RayTriangleIntersect(&pointOnPlane, &terrainClip->m_f, &terrainClip->m_e, &terrainClip->m_d))
			{
				m_camera->m_position.x = pointOnPlane.p.x;
				m_camera->m_position.y = pointOnPlane.p.y + 2.0f;
				m_camera->m_position.z = pointOnPlane.p.z;

				return true;
			}
		}
	}

	return false;
}
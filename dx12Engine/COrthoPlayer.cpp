#include "COrthoPlayer.h"

COrthoPlayer::COrthoPlayer()
{
	memset(this, 0x00, sizeof(COrthoPlayer));
}

COrthoPlayer::COrthoPlayer(CGlobalObjects* globalObjects, COrthoLevel* orthoLevel, float rate, CVertex2* cellSize, CVertex2* scaleSize, CVertex2* startPosition, const char* name)
{
	memset(this, 0x00, sizeof(COrthoPlayer));

	m_globalObjects = globalObjects;

	m_level = orthoLevel;
	m_rate = rate;

	m_cellSize = *cellSize;

	m_position = *startPosition;

	m_coordinates = *startPosition;

	m_name = new CString(name);

	m_scaleSize.p.x = scaleSize->p.x * (UINT)((float)m_globalObjects->m_window->m_width / (float)m_globalObjects->m_window->m_height);
	m_scaleSize.p.y = scaleSize->p.y * (UINT)((float)m_globalObjects->m_window->m_width / (float)m_globalObjects->m_window->m_height);

	CVertex2 v1(2048.0f, 1536.0f);
	CVertex2 v2(0.0f, 4.0f);
}

COrthoPlayer::~COrthoPlayer()
{
	delete m_name;
	delete m_image;
}

void COrthoPlayer::PlayerMove(CEvent* evt)
{
	m_direction.p.x = 0.0f;
	m_direction.p.y = 0.0f;

	m_distance = m_rate * m_globalObjects->m_frametime->m_frametime;

	switch (evt->m_type)
	{
	case CEvent::ET_MOVE_UP:
	{
		m_direction.p.y = -1.0f;

		break;
	}

	case CEvent::ET_MOVE_DOWN:
	{
		m_direction.p.y = 1.0f;

		break;
	}

	case CEvent::ET_MOVE_LEFT:
	{
		m_direction.p.x = -1.0f;

		break;
	}

	case CEvent::ET_MOVE_RIGHT:
	{
		m_direction.p.x = 1.0f;

		break;
	}
	}

	m_direction.RadiusNormalize();

	CVertex2 pp = m_position;

	m_position.p.x += m_direction.p.x * m_distance;
	m_position.p.y += m_direction.p.y * m_distance;

	m_image->m_position = m_position;
	m_image->UpdatePosition();

	if (COrthoPlayer::CheckLevelCollision())
	{
		m_position = pp;
	}
	else
	{
		m_coordinates.p.x += (m_direction.p.x * m_distance);
		m_coordinates.p.y += (m_direction.p.y * m_distance);
	}


	if (m_position.p.x < (m_image->m_scaleSize.p.x / 2.0f))
	{
		m_position.p.x = (m_image->m_scaleSize.p.x / 2.0f);
	}

	if (m_position.p.y < (m_image->m_scaleSize.p.y / 2.0f))
	{
		m_position.p.y = (m_image->m_scaleSize.p.y / 2.0f);
	}

	if (m_globalObjects->m_window->m_windowed)
	{
		if (m_position.p.x > m_globalObjects->m_window->m_width - (m_image->m_scaleSize.p.x / 2.0f))
		{
			m_position.p.x = m_globalObjects->m_window->m_width - (m_image->m_scaleSize.p.x / 2.0f);
		}

		if (m_position.p.y > m_globalObjects->m_window->m_height - (m_image->m_scaleSize.p.y / 2.0f))
		{
			m_position.p.y = m_globalObjects->m_window->m_height - (m_image->m_scaleSize.p.y / 2.0f);
		}
	}
	else
	{
		if (m_position.p.x > m_globalObjects->m_window->m_width - (m_image->m_scaleSize.p.x / 2.0f))
		{
			m_position.p.x = m_globalObjects->m_window->m_width - (m_image->m_scaleSize.p.x / 2.0f);
		}

		if (m_position.p.y > m_globalObjects->m_window->m_height - (m_image->m_scaleSize.p.y / 2.0f))
		{
			m_position.p.y = m_globalObjects->m_window->m_height - (m_image->m_scaleSize.p.y / 2.0f);
		}
	}

	m_image->m_position = m_position;
	
	m_image->UpdatePosition();
}

BYTE COrthoPlayer::CheckLevelCollision()
{
	for (UINT y = 0; y < m_level->m_height; y++)
	{
		for (UINT x = 0; x < m_level->m_width; x++)
		{
			COrthoTile* tile = (COrthoTile*)m_level->m_tiles->GetElement(2, x, y);

			if (tile)
			{
				if (tile->m_type == COrthoTile::OT_CLIP)
				{
					CVertex v(m_image->m_box->m_mins.p.x, m_image->m_box->m_mins.p.y, 0.0f);

					if (tile->m_box->CheckPointInBox(&v))
					{
						return true;
					}

					v = CVertex(m_image->m_box->m_maxs.p.x, m_image->m_box->m_mins.p.y, 0.0f);

					if (tile->m_box->CheckPointInBox(&v))
					{
						return true;
					}

					v = CVertex(m_image->m_box->m_mins.p.x, m_image->m_box->m_maxs.p.y, 0.0f);

					if (tile->m_box->CheckPointInBox(&v))
					{
						return true;
					}

					v = CVertex(m_image->m_box->m_maxs.p.x, m_image->m_box->m_maxs.p.y, 0.0f);

					if (tile->m_box->CheckPointInBox(&v))
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}
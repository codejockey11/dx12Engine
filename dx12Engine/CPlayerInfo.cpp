#include "CPlayerInfo.h"

/*
*/
CPlayerInfo::CPlayerInfo()
{
	memset(this, 0x00, sizeof(CPlayerInfo));

	m_acceleration = 8.0f;
}

/*
*/
CPlayerInfo::~CPlayerInfo()
{
}

/*
*/
void CPlayerInfo::Initialize(CPlayerInfo* playerInfo)
{
	if (strlen(playerInfo->m_name) == 0)
	{
		return;
	}

	memcpy_s((void*)m_name, CPlayerInfo::E_NAME_SIZE, (void*)playerInfo->m_name, strlen(playerInfo->m_name));

	m_team = playerInfo->m_team;

	m_position = playerInfo->m_position;
	m_direction = playerInfo->m_direction;

	m_acceleration = playerInfo->m_acceleration;

	m_state = playerInfo->m_state;

	m_isRecordable = playerInfo->m_isRecordable;

	m_camera = playerInfo->m_camera;

	m_model = playerInfo->m_model;
}

/*
*/
void CPlayerInfo::SetDirection(XMFLOAT3* direction)
{
	m_direction.p.x = direction->x;
	m_direction.p.y = direction->y;
	m_direction.p.z = direction->z;
}

/*
*/
void CPlayerInfo::SetName(const char* name)
{
	memcpy_s((void*)m_name, CPlayerInfo::E_NAME_SIZE, (void*)name, strlen(name));
}
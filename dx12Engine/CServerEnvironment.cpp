#include "CServerEnvironment.h"

CServerEnvironment::CServerEnvironment()
{
	memset(this, 0x00, sizeof(CServerEnvironment));
}

CServerEnvironment::CServerEnvironment(const char* filename)
{
	memset(this, 0x00, sizeof(CServerEnvironment));

	m_redTeamStart[0] = new CTeamStart(true, CVertex(-8.0f, 2.0f, -8.0f), CVertex(0.0f, 359.0f, 0.0f));
	m_redTeamStart[1] = new CTeamStart(true, CVertex(-4.0f, 2.0f, -8.0f), CVertex(0.0f, 359.0f, 0.0f));
	m_redTeamStart[2] = new CTeamStart(true, CVertex(0.0f, 2.0f, -8.0f), CVertex(0.0f, 359.0f, 0.0f));
	m_redTeamStart[3] = new CTeamStart(true, CVertex(4.0f, 2.0f, -8.0f), CVertex(0.0f, 359.0f, 0.0f));
	m_redTeamStart[4] = new CTeamStart(true, CVertex(8.0f, 2.0f, -8.0f), CVertex(0.0f, 359.0f, 0.0f));

	m_blueTeamStart[0] = new CTeamStart(true, CVertex(-8.0f, 2.0f, 8.0f), CVertex(0.0f, 180.0f, 0.0f));
	m_blueTeamStart[1] = new CTeamStart(true, CVertex(-4.0f, 2.0f, 8.0f), CVertex(0.0f, 180.0f, 0.0f));
	m_blueTeamStart[2] = new CTeamStart(true, CVertex(0.0f, 2.0f, 8.0f), CVertex(0.0f, 180.0f, 0.0f));
	m_blueTeamStart[3] = new CTeamStart(true, CVertex(4.0f, 2.0f, 8.0f), CVertex(0.0f, 180.0f, 0.0f));
	m_blueTeamStart[4] = new CTeamStart(true, CVertex(8.0f, 2.0f, 8.0f), CVertex(0.0f, 180.0f, 0.0f));

	FILE* file = {};

	errno_t err = fopen_s(&file, "terrain\\clips.txt", "rb");

	if (err != 0)
	{
		return;
	}

	size_t bytesRead = 0;

	bytesRead = fread_s(&m_width, sizeof(int), sizeof(int), 1, file);
	bytesRead = fread_s(&m_height, sizeof(int), sizeof(int), 1, file);
	bytesRead = fread_s(&m_primSize, sizeof(int), sizeof(int), 1, file);

	if (file)
	{
		m_clips = new CHeapArray(sizeof(CTerrainClip), true, false, 2, m_width - 1, m_height - 1);

		size_t bytesRead = fread_s(m_clips->m_heap, (size_t)(sizeof(CTerrainClip) * (size_t)(m_width - 1) * (size_t)(m_height - 1)), sizeof(CTerrainClip), (size_t)(m_width - 1) * (size_t)(m_height - 1), file);

		fclose(file);
	}
}

CServerEnvironment::~CServerEnvironment()
{
	delete m_redTeamStart[0];
	delete m_redTeamStart[1];
	delete m_redTeamStart[2];
	delete m_redTeamStart[3];
	delete m_redTeamStart[4];
	
	delete m_blueTeamStart[0];
	delete m_blueTeamStart[1];
	delete m_blueTeamStart[2];
	delete m_blueTeamStart[3];
	delete m_blueTeamStart[4];

	delete m_clips;
}
#include "CEvent.h"

CEvent::CEvent()
{
	memset(this, 0x00, sizeof(CEvent));
}

CEvent::CEvent(CEvent* evt)
{
	memset(this, 0x00, sizeof(CEvent));

	m_fromClient = evt->m_fromClient;
	m_toClient = evt->m_toClient;
	m_audience = evt->m_audience;
	m_type = evt->m_type;

	m_length = evt->m_length;

	if (m_length > CEvent::DATA_LENGTH)
	{
		m_length = CEvent::DATA_LENGTH;
	}

	memcpy((void*)m_data, (void*)evt->m_data, m_length);

	memcpy((void*)m_playerInfo, (void*)evt->m_playerInfo, CEvent::DATA_LENGTH);
}

CEvent::CEvent(int fromClient, int toClient, BYTE audience, BYTE type)
{
	memset(this, 0x00, sizeof(CEvent));

	m_fromClient = fromClient;
	m_toClient = toClient;
	m_audience = audience;
	m_type = type;
}

CEvent::CEvent(int fromClient, int toClient, BYTE audience, BYTE type, BYTE data, int length)
{
	memset(this, 0x00, sizeof(CEvent));

	m_fromClient = fromClient;
	m_toClient = toClient;
	m_audience = audience;
	m_type = type;

	m_length = length;

	m_data[0] = data;
}

CEvent::CEvent(int fromClient, int toClient, BYTE audience, BYTE type, BYTE* data, int length)
{
	memset(this, 0x00, sizeof(CEvent));

	m_fromClient = fromClient;
	m_toClient = toClient;
	m_audience = audience;
	m_type = type;

	m_length = length;

	if (m_length > CEvent::DATA_LENGTH)
	{
		m_length = CEvent::DATA_LENGTH;
	}

	memcpy((void*)m_data, (void*)data, m_length);
}

CEvent::CEvent(int fromClient, int toClient, BYTE audience, BYTE type, const char* data, int length)
{
	memset(this, 0x00, sizeof(CEvent));

	m_fromClient = fromClient;
	m_toClient = toClient;
	m_audience = audience;
	m_type = type;

	m_length = length;

	if (m_length > CEvent::DATA_LENGTH)
	{
		m_length = CEvent::DATA_LENGTH;
	}

	memcpy((void*)m_data, (void*)data, m_length);
}

CEvent::CEvent(int fromClient, int toClient, BYTE audience, BYTE type, void* data, int length, void* playerInfo)
{
	memset(this, 0x00, sizeof(CEvent));

	m_fromClient = fromClient;
	m_toClient = toClient;
	m_audience = audience;
	m_type = type;

	m_length = length;

	if (m_length > CEvent::DATA_LENGTH)
	{
		m_length = CEvent::DATA_LENGTH;
	}

	memcpy((void*)m_data, (void*)data, m_length);

	memcpy((void*)m_playerInfo, (void*)playerInfo, CEvent::DATA_LENGTH);
}

CEvent::~CEvent()
{
}

void CEvent::Clear()
{
	memset((void*)m_data, 0x00, CEvent::DATA_LENGTH);
}
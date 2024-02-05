#include "CConfigurationItem.h"

/*
*/
CConfigurationItem::CConfigurationItem()
{
	memset(this, 0x00, sizeof(CConfigurationItem));
}

/*
*/
CConfigurationItem::CConfigurationItem(float value)
{
	m_value = value;
}

/*
*/
CConfigurationItem::~CConfigurationItem()
{
}
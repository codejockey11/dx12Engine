#pragma once

#include "standard.h"

#include "CConfigurationItem.h"
#include "CLinkList.h"
#include "CString.h"

class CConfiguration
{
public:

	enum
	{
		E_CF_HERTZ = 0,
		E_CF_MAXSTRING,
		E_CF_MOUSESENSITIVITY,
		E_CF_SCREENHEIGHT,
		E_CF_SCREENWIDTH,
		E_CF_WINDOWED,
		E_CF_MAXENTRIES,

		E_BUFFER_SIZE = 80
	};

	CString* m_filename;
	CConfigurationItem m_items[CConfiguration::E_CF_MAXENTRIES];

	CConfiguration();
	CConfiguration(const char* filename);
	~CConfiguration();
};
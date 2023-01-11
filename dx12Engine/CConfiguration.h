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
		CF_SCREENWIDTH = 0,
		CF_SCREENHEIGHT,
		CF_HERTZ,
		CF_MAXSTRING,
		CF_WINDOWED,
		CF_MOUSESENSITIVITY,

		CF_MAXENTRIES,

		BUFFERSIZE = 80
	};

	CString* m_filename;

	CConfigurationItem m_items[CConfiguration::CF_MAXENTRIES];


	CConfiguration();

	CConfiguration(const char* filename);

	~CConfiguration();
};
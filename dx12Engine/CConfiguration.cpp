#include "CConfiguration.h"

/*
*/
CConfiguration::CConfiguration()
{
	memset(this, 0x00, sizeof(CConfiguration));
}

/*
*/
CConfiguration::CConfiguration(const char* filename)
{
	memset(this, 0x00, sizeof(CConfiguration));

	m_filename = new CString(filename);

	FILE* file = {};

	errno_t err = fopen_s(&file, m_filename->GetText(), "r");

	if (err == 0)
	{
		char fileBuffer[CConfiguration::E_BUFFER_SIZE] = {};

		float value;

		err = fscanf_s(file, "%s %f\n", fileBuffer, CConfiguration::E_BUFFER_SIZE, &value);

		while (err != 0xFFFFFFFF)
		{
			if (strcmp(fileBuffer, "hertz") == 0)
			{
				m_items[CConfiguration::E_CF_HERTZ].m_value = value;
			}
			else if (strcmp(fileBuffer, "maxString") == 0)
			{
				m_items[CConfiguration::E_CF_MAXSTRING].m_value = value;
			}
			else if (strcmp(fileBuffer, "mouseSensitivity") == 0)
			{
				m_items[CConfiguration::E_CF_MOUSESENSITIVITY].m_value = value;
			}
			else if (strcmp(fileBuffer, "screenHeight") == 0)
			{
				m_items[CConfiguration::E_CF_SCREENHEIGHT].m_value = value;
			}
			else if (strcmp(fileBuffer, "screenWidth") == 0)
			{
				m_items[CConfiguration::E_CF_SCREENWIDTH].m_value = value;
			}
			else if (strcmp(fileBuffer, "windowed") == 0)
			{
				m_items[CConfiguration::E_CF_WINDOWED].m_value = value;
			}

			err = fscanf_s(file, "%s %f\n", fileBuffer, CConfiguration::E_BUFFER_SIZE, &value);
		}
	}

	if (file)
	{
		fclose(file);
	}
}

/*
*/
CConfiguration::~CConfiguration()
{
	delete m_filename;
}
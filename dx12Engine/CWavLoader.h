#pragma once

#include "standard.h"

#include "CErrorLog.h"
#include "CString.h"
#include "CWavFileHeader.h"

class CWavLoader
{
public:

	CString* m_filename;

	CWavFileHeader m_header;

	BYTE* m_data;

	WAVEFORMATEX m_wfx;


	CWavLoader();

	CWavLoader(CErrorLog* errorLog, const char* filename);

	~CWavLoader();

private:

	CErrorLog* m_errorLog;
};
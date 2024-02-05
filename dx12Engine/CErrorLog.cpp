#include "CErrorLog.h"

/*
*/
CErrorLog::CErrorLog()
{
	memset(this, 0x00, sizeof(CErrorLog));
}

/*
*/
CErrorLog::CErrorLog(const char* filename)
{
	memset(this, 0x00, sizeof(CErrorLog));

	fopen_s(&m_file, filename, "w");

	m_comError = new CComError();

	m_wsError = new CWinsockError();

	m_dxgiError = new CDXGIError();
}

/*
*/
CErrorLog::~CErrorLog()
{
	delete m_comError;
	delete m_wsError;
	delete m_dxgiError;

	if (m_file)
	{
		fclose(m_file);
	}
}

/*
*/
CDXGIErrorItem* CErrorLog::GetDXGIErrorMessage(UINT error)
{
	return m_dxgiError->GetError(error);
}

/*
*/
CWinsockErrorItem* CErrorLog::GetWinsockErrorMessage(UINT error)
{
	return m_wsError->GetError(error);
}

/*
*/
char* CErrorLog::GetComErrorMessage(HRESULT hr)
{
	return m_comError->GetComErrorMessage(hr);
}

/*
*/
void CErrorLog::WriteBytes(const char* bytes)
{
	for (int i = 0; i < strlen(bytes); i++)
	{
		memset((void*)m_text, 0x00, CErrorLog::E_LINE_LENGTH);

		sprintf_s(m_text, 2, "%c", bytes[i]);

		if (m_file)
		{
			fwrite(m_text, 1, 1, m_file);
		}
	}

	if (m_file)
	{
		fwrite("<WriteBytesEndLine>\n", 1, 20, m_file);
	}

	fflush(m_file);
}

/*
*/
void CErrorLog::WriteError(const char* format, ...)
{
	if (strlen(format) == 0)
	{
		return;
	}

	SYSTEMTIME st = {};

	char timeText[CErrorLog::E_TIME_LENGTH] = {};

	GetLocalTime(&st);

	sprintf_s(timeText, CErrorLog::E_TIME_LENGTH, "%02d:%02d:%02d - ", st.wHour, st.wMinute, st.wSecond);

#ifdef _DEBUG
	OutputDebugStringA(timeText);
#endif

	va_list argptr;

	va_start(argptr, format);

	vsprintf_s(m_text, CErrorLog::E_LINE_LENGTH, format, argptr);

	va_end(argptr);

#ifdef _DEBUG
	OutputDebugStringA(m_text);
#endif

	if (m_file)
	{
		fwrite(timeText, strlen(timeText), 1, m_file);

		fflush(m_file);

		fwrite(m_text, strlen(m_text), 1, m_file);

		fflush(m_file);
	}

	memset((void*)m_text, 0x00, CErrorLog::E_LINE_LENGTH);
}